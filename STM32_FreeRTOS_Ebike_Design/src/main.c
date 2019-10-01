/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "Task.h"

#define FALSE 1
#define TRUE 0

#define NOT_PRESSED FALSE
#define PRESSED     TRUE

#define STANDARD 0xAA
#define ECO 	 0xBB
#define TOUR	 0xCC
#define SPORT	 0xDD
#define TURBO 	 0xEE

uint8_t button_status_flag = NOT_PRESSED;

uint32_t State = STANDARD;

void vApplicationIdleHook(void)
{
	//send the CPU to normal sleep mode
	__WFI();
}

static void prvSetupUart(void)
{
	GPIO_InitTypeDef gpio_uart_pins;
	USART_InitTypeDef uart2_init;

	// 1. Enable UART2 and GPIO A peripheral clock because we are using UART 2 for virtual com port communication
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); // Enables or disables the low speed peripheral clock

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	//PA2 acts as UART2-Tx and PA3 acts as UART2-Rx when configured in alternate function mode-7
	// 2. Alternate function configuration of MCU pins to behave as UART2 Tx and Rx -- goto GPIO for that to configure pins

	// Better to initialize all memeber elements to "0"
	// zeroing each and every member element of the structure
	memset(&gpio_uart_pins, 0, sizeof(gpio_uart_pins));

	gpio_uart_pins.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	gpio_uart_pins.GPIO_Mode = GPIO_Mode_AF;
	gpio_uart_pins.GPIO_PuPd = GPIO_PuPd_UP; // pullup because whenever the line is Idle, then it's pulled to "1"

	// Configure GPIOA to behave as alternate function
	GPIO_Init(GPIOA, &gpio_uart_pins);

	// We haven't still configured who is Tx and who is Rx. We just configured pins as Alternate functionality
	// 3. AF mode settings for the pins - stating who is Tx and Who is Rx
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2); // PA2
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2); // PA3

	// 4. UART parameter initilaizations - this is UART peripheral config, so far we did UART PIN config

	memset(&uart2_init, 0, sizeof(uart2_init));

	uart2_init.USART_BaudRate = 115200;
	uart2_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	uart2_init.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	uart2_init.USART_Parity = USART_Parity_No;
	uart2_init.USART_StopBits = USART_StopBits_1;
	uart2_init.USART_WordLength = USART_WordLength_8b;

	USART_Init(USART2, &uart2_init);

	// 5. Enable the UART2 peripheral
	USART_Cmd(USART2, ENABLE);
}

void prvSetupGpio()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);


	// this function is board specific
	GPIO_InitTypeDef led_init, button_init;

	led_init.GPIO_Mode = GPIO_Mode_OUT;
	led_init.GPIO_OType = GPIO_OType_PP;
	led_init.GPIO_Pin = GPIO_Pin_5;
	led_init.GPIO_Speed = GPIO_Low_Speed;
	led_init.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOA, &led_init);

	button_init.GPIO_Mode = GPIO_Mode_IN;
	button_init.GPIO_OType = GPIO_OType_PP;
	button_init.GPIO_Pin = GPIO_Pin_13;
	button_init.GPIO_Speed = GPIO_Low_Speed;
	button_init.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOC, &button_init);

	// Interrupt configuration for Button (PC13)
	// configure EXTI line by selecting pin in SYSCFG_EXTICR
	//1. system configuration for EXTI line (SYSCFG settings)
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource13);

	// Configuring only SYSCFG is not sufficient and we need to configure the whole EXTI block

	//2. EXTI Line configuration 13, falling,interrupt mode
	EXTI_InitTypeDef exti_init;
	exti_init.EXTI_Line = EXTI_Line13;
	exti_init.EXTI_Mode = EXTI_Mode_Interrupt;
	exti_init.EXTI_Trigger = EXTI_Trigger_Falling;
	exti_init.EXTI_LineCmd = ENABLE;

	EXTI_Init(&exti_init);

	//3. NVIC settings (IRQ settings for the selected EXTI line(13))IRQ 40(EXTI 10 - 15)
	NVIC_SetPriority(EXTI15_10_IRQn, 5);
	NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void rtos_delay(uint32_t delay_in_ms)
{
	// take tick count value of free rtos to implement delay
	uint32_t current_tick_count;
	uint32_t delay_in_ticks;

	current_tick_count = xTaskGetTickCount();

	delay_in_ticks = (delay_in_ms * configTICK_RATE_HZ) / 1000;

	while(xTaskGetTickCount() < (current_tick_count + delay_in_ticks));

}

void button_handler(void *params)
{
	button_status_flag ^=1;
}

void switch_state(void *params)
{
	switch(State)
	{
	  case STANDARD:
		  State = ECO;
		  break;
	  case ECO:
	  	  State = TOUR;
	  	  break;
	  case TOUR:
	  	  State = SPORT;
	  	  break;
	  case SPORT:
	  	  State = TURBO;
	  	  break;
	  case TURBO:
	  	  State = STANDARD;
	  	  break;
	  default:
		  break;
	}
}

void EXTI15_10_IRQHandler(void)
{
	//1. clear the interrupt pending bit of the EXTI line (13)
	EXTI_ClearITPendingBit(EXTI_Line13);

	button_handler(NULL);
	switch_state(NULL);
}

static void prvSetupHardware(void)
{
	// Setup Button and led
	prvSetupGpio();

	// Setup UART
	prvSetupUart();
}

void printmsg(char *msg)
{
	// But here we have to wait till the data transmission has ended and then send another byte otherwise we will corrupt the UART
	for(uint32_t i=0; i < strlen(msg); i++)
	{
		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) != SET);

		USART_SendData(USART2, msg[i]);
	}
}

void led_task_handler(void *params)
{
	while(1)
	{
		/*if(button_status_flag == PRESSED)
		{
			//turn on led / write bit because we are talking to specific pin of output port
			GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_SET);
		}
		else
		{
			// turn off led
			GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_RESET);
		}*/

		if (State == STANDARD)
		{
			rtos_delay(2000);
			GPIO_ToggleBits(GPIOA, GPIO_Pin_5);
		}
		if (State == ECO)
		{
			rtos_delay(1000);
			GPIO_ToggleBits(GPIOA, GPIO_Pin_5);
		}
		if (State == TOUR)
		{
			rtos_delay(500);
			GPIO_ToggleBits(GPIOA, GPIO_Pin_5);
		}
		if (State == SPORT)
		{
			rtos_delay(200);
			GPIO_ToggleBits(GPIOA, GPIO_Pin_5);
		}
		if (State == TURBO)
		{
			rtos_delay(100);
			GPIO_ToggleBits(GPIOA, GPIO_Pin_5);
		}
	}
}

int main(void)
{
	// 1. REset Rcc clock configuration to default reset state.
	// HSI - on; PLL - OFF; HSE - OFF; system clk = 16 Mhz, cou_clock = 16 Mhz
	RCC_DeInit();

	//SystemCoreClock = 16000000; // can also do like this

	// 2. Update system core clock variable
	SystemCoreClockUpdate();

	prvSetupHardware();

	// lets create LED task
	xTaskCreate(led_task_handler, "LED-Task", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

	//xTaskCreate(button_task_handler, "Button-Task", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

	// 4. Invoke Scheduler
	vTaskStartScheduler();

	for(;;);
}
