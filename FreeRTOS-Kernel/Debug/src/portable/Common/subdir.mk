################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/portable/Common/mpu_wrappers.c \
../src/portable/Common/mpu_wrappers_v2.c 

C_DEPS += \
./src/portable/Common/mpu_wrappers.d \
./src/portable/Common/mpu_wrappers_v2.d 

OBJS += \
./src/portable/Common/mpu_wrappers.o \
./src/portable/Common/mpu_wrappers_v2.o 


# Each subdirectory must supply rules for building sources it contributes
src/portable/Common/%.o: ../src/portable/Common/%.c src/portable/Common/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__NEWLIB__ -I"C:\LEETC\SE\MCUXpressoIDE_11.10.0_3148\workspace\NSP\FreeRTOS-Template\inc" -I"C:\LEETC\SE\MCUXpressoIDE_11.10.0_3148\workspace\NSP\FreeRTOS-Kernel\include" -I"C:\LEETC\SE\MCUXpressoIDE_11.10.0_3148\workspace\NSP\FreeRTOS-Kernel\src\portable\GCC\ARM_CM3" -I"C:\LEETC\SE\MCUXpressoIDE_11.10.0_3148\workspace\NSP\CMSIS_CORE_LPC17xx\inc" -O0 -g3 -gdwarf-4 -Wall -c -fmessage-length=0 -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m3 -mthumb -D__NEWLIB__ -fstack-usage -specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-portable-2f-Common

clean-src-2f-portable-2f-Common:
	-$(RM) ./src/portable/Common/mpu_wrappers.d ./src/portable/Common/mpu_wrappers.o ./src/portable/Common/mpu_wrappers_v2.d ./src/portable/Common/mpu_wrappers_v2.o

.PHONY: clean-src-2f-portable-2f-Common

