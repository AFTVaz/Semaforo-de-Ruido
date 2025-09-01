################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/buzzer.c \
../src/delay.c \
../src/flashcontrol.c \
../src/led.c \
../src/max9814.c \
../src/rtc.c \
../src/transport.c 

C_DEPS += \
./src/buzzer.d \
./src/delay.d \
./src/flashcontrol.d \
./src/led.d \
./src/max9814.d \
./src/rtc.d \
./src/transport.d 

OBJS += \
./src/buzzer.o \
./src/delay.o \
./src/flashcontrol.o \
./src/led.o \
./src/max9814.o \
./src/rtc.o \
./src/transport.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSIS_CORE_LPC17xx -D__LPC17XX__ -D__NEWLIB__ -I"C:\LEETC\SE\MCUXpressoIDE_11.10.0_3148\workspace\NSP\DRIVERS\inc" -I"C:\LEETC\SE\MCUXpressoIDE_11.10.0_3148\workspace\NSP\HAL\inc" -I"C:\LEETC\SE\MCUXpressoIDE_11.10.0_3148\workspace\NSP\CMSIS_CORE_LPC17xx\inc" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m3 -mthumb -D__NEWLIB__ -fstack-usage -specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/buzzer.d ./src/buzzer.o ./src/delay.d ./src/delay.o ./src/flashcontrol.d ./src/flashcontrol.o ./src/led.d ./src/led.o ./src/max9814.d ./src/max9814.o ./src/rtc.d ./src/rtc.o ./src/transport.d ./src/transport.o

.PHONY: clean-src

