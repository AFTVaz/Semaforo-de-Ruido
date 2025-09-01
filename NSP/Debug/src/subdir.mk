################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/NSP.c \
../src/cr_startup_lpc175x_6x-FreeRTOS.c \
../src/crp.c \
../src/printf-stdarg.c 

C_DEPS += \
./src/NSP.d \
./src/cr_startup_lpc175x_6x-FreeRTOS.d \
./src/crp.d \
./src/printf-stdarg.d 

OBJS += \
./src/NSP.o \
./src/cr_startup_lpc175x_6x-FreeRTOS.o \
./src/crp.o \
./src/printf-stdarg.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSIS_CORE_LPC17xx -D__LPC17XX__ -D__NEWLIB__ -DxSERIAL_CONSOLE -I"C:\LEETC\SE\MCUXpressoIDE_11.10.0_3148\workspace\NSP\NSP\inc" -I"C:\LEETC\SE\MCUXpressoIDE_11.10.0_3148\workspace\NSP\MQTTPacket\inc" -I"C:\LEETC\SE\MCUXpressoIDE_11.10.0_3148\workspace\NSP\FreeRTOS_DRIVERS\inc" -I"C:\LEETC\SE\MCUXpressoIDE_11.10.0_3148\workspace\NSP\DRIVERS\inc" -I"C:\LEETC\SE\MCUXpressoIDE_11.10.0_3148\workspace\NSP\CMSIS_CORE_LPC17xx\inc" -I"C:\LEETC\SE\MCUXpressoIDE_11.10.0_3148\workspace\NSP\FreeRTOS-Kernel\include" -I"C:\LEETC\SE\MCUXpressoIDE_11.10.0_3148\workspace\NSP\FreeRTOS-Kernel\src\portable\GCC\ARM_CM3" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m3 -mthumb -D__NEWLIB__ -fstack-usage -specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/NSP.d ./src/NSP.o ./src/cr_startup_lpc175x_6x-FreeRTOS.d ./src/cr_startup_lpc175x_6x-FreeRTOS.o ./src/crp.d ./src/crp.o ./src/printf-stdarg.d ./src/printf-stdarg.o

.PHONY: clean-src

