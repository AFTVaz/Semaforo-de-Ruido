################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/buzzer_rtos.c \
../src/clock.c \
../src/flash_rtos.c \
../src/led_rtos.c \
../src/microphone.c \
../src/mqtt.c \
../src/ntp.c \
../src/wifi.c 

C_DEPS += \
./src/buzzer_rtos.d \
./src/clock.d \
./src/flash_rtos.d \
./src/led_rtos.d \
./src/microphone.d \
./src/mqtt.d \
./src/ntp.d \
./src/wifi.d 

OBJS += \
./src/buzzer_rtos.o \
./src/clock.o \
./src/flash_rtos.o \
./src/led_rtos.o \
./src/microphone.o \
./src/mqtt.o \
./src/ntp.o \
./src/wifi.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSIS_CORE_LPC17xx -D__LPC17XX__ -D__NEWLIB__ -I"C:\LEETC\SE\MCUXpressoIDE_11.10.0_3148\workspace\NSP\FreeRTOS_DRIVERS\inc" -I"C:\LEETC\SE\MCUXpressoIDE_11.10.0_3148\workspace\NSP\MQTTPacket\inc" -I"C:\LEETC\SE\MCUXpressoIDE_11.10.0_3148\workspace\NSP\FreeRTOS-Kernel\include" -I"C:\LEETC\SE\MCUXpressoIDE_11.10.0_3148\workspace\NSP\FreeRTOS-Kernel\src\portable\GCC\ARM_CM3" -I"C:\LEETC\SE\MCUXpressoIDE_11.10.0_3148\workspace\NSP\FreeRTOS-Template\inc" -I"C:\LEETC\SE\MCUXpressoIDE_11.10.0_3148\workspace\NSP\DRIVERS\inc" -I"C:\LEETC\SE\MCUXpressoIDE_11.10.0_3148\workspace\NSP\CMSIS_CORE_LPC17xx\inc" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m3 -mthumb -D__NEWLIB__ -fstack-usage -specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/buzzer_rtos.d ./src/buzzer_rtos.o ./src/clock.d ./src/clock.o ./src/flash_rtos.d ./src/flash_rtos.o ./src/led_rtos.d ./src/led_rtos.o ./src/microphone.d ./src/microphone.o ./src/mqtt.d ./src/mqtt.o ./src/ntp.d ./src/ntp.o ./src/wifi.d ./src/wifi.o

.PHONY: clean-src

