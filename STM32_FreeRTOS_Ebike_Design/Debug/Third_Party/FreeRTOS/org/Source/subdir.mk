################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Third_Party/FreeRTOS/org/Source/croutine.c \
../Third_Party/FreeRTOS/org/Source/event_groups.c \
../Third_Party/FreeRTOS/org/Source/list.c \
../Third_Party/FreeRTOS/org/Source/queue.c \
../Third_Party/FreeRTOS/org/Source/stream_buffer.c \
../Third_Party/FreeRTOS/org/Source/tasks.c \
../Third_Party/FreeRTOS/org/Source/timers.c 

OBJS += \
./Third_Party/FreeRTOS/org/Source/croutine.o \
./Third_Party/FreeRTOS/org/Source/event_groups.o \
./Third_Party/FreeRTOS/org/Source/list.o \
./Third_Party/FreeRTOS/org/Source/queue.o \
./Third_Party/FreeRTOS/org/Source/stream_buffer.o \
./Third_Party/FreeRTOS/org/Source/tasks.o \
./Third_Party/FreeRTOS/org/Source/timers.o 

C_DEPS += \
./Third_Party/FreeRTOS/org/Source/croutine.d \
./Third_Party/FreeRTOS/org/Source/event_groups.d \
./Third_Party/FreeRTOS/org/Source/list.d \
./Third_Party/FreeRTOS/org/Source/queue.d \
./Third_Party/FreeRTOS/org/Source/stream_buffer.d \
./Third_Party/FreeRTOS/org/Source/tasks.d \
./Third_Party/FreeRTOS/org/Source/timers.d 


# Each subdirectory must supply rules for building sources it contributes
Third_Party/FreeRTOS/org/Source/%.o: ../Third_Party/FreeRTOS/org/Source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32 -DSTM32F4 -DSTM32F411RETx -DNUCLEO_F411RE -DDEBUG -DSTM32F411xE -DUSE_STDPERIPH_DRIVER -I"C:/Users/Polepall/Documents/FreeRtos/RTOS_Workspace/STM32_FreeRTOS_Ebike_Design/StdPeriph_Driver/inc" -I"C:/Users/Polepall/Documents/FreeRtos/RTOS_Workspace/STM32_FreeRTOS_Ebike_Design/Config" -I"C:/Users/Polepall/Documents/FreeRtos/RTOS_Workspace/STM32_FreeRTOS_Ebike_Design/Third_Party/FreeRTOS/org/Source/include" -I"C:/Users/Polepall/Documents/FreeRtos/RTOS_Workspace/STM32_FreeRTOS_Ebike_Design/Third_Party/FreeRTOS/org/Source/portable/GCC/ARM_CM4F" -I"C:/Users/Polepall/Documents/FreeRtos/RTOS_Workspace/STM32_FreeRTOS_Ebike_Design/inc" -I"C:/Users/Polepall/Documents/FreeRtos/RTOS_Workspace/STM32_FreeRTOS_Ebike_Design/CMSIS/device" -I"C:/Users/Polepall/Documents/FreeRtos/RTOS_Workspace/STM32_FreeRTOS_Ebike_Design/CMSIS/core" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


