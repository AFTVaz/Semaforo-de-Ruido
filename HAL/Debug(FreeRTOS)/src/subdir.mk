################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/adc.c \
../src/flash.c \
../src/pin.c \
../src/pinconfig.c \
../src/systick.c \
../src/timer2.c 

C_DEPS += \
./src/adc.d \
./src/flash.d \
./src/pin.d \
./src/pinconfig.d \
./src/systick.d \
./src/timer2.d 

OBJS += \
./src/adc.o \
./src/flash.o \
./src/pin.o \
./src/pinconfig.o \
./src/systick.o \
./src/timer2.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSIS_CORE_LPC17xx -D__LPC17XX__ -D__NEWLIB__ -I"C:\LEETC\SE\MCUXpressoIDE_11.10.0_3148\workspace\NSP\HAL\inc" -I"C:\LEETC\SE\MCUXpressoIDE_11.10.0_3148\workspace\NSP\FreeRTOS-Kernel\include" -I"C:\LEETC\SE\MCUXpressoIDE_11.10.0_3148\workspace\NSP\FreeRTOS-Template\inc" -I"C:\LEETC\SE\MCUXpressoIDE_11.10.0_3148\workspace\NSP\FreeRTOS-Kernel\src\portable\GCC\ARM_CM3" -I"C:\LEETC\SE\MCUXpressoIDE_11.10.0_3148\workspace\NSP\CMSIS_CORE_LPC17xx\inc" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m3 -mthumb -D__NEWLIB__ -fstack-usage -specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/adc.d ./src/adc.o ./src/flash.d ./src/flash.o ./src/pin.d ./src/pin.o ./src/pinconfig.d ./src/pinconfig.o ./src/systick.d ./src/systick.o ./src/timer2.d ./src/timer2.o

.PHONY: clean-src

