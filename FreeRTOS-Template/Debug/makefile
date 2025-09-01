################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := FreeRTOS-Template
BUILD_ARTIFACT_EXTENSION := axf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all:
	+@$(MAKE) --no-print-directory main-build && $(MAKE) --no-print-directory post-build

# Main-build Target
main-build: FreeRTOS-Template.axf

# Tool invocations
FreeRTOS-Template.axf: $(OBJS) $(USER_OBJS) makefile $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\LEETC\SE\MCUXpressoIDE_11.10.0_3148\workspace\NSP\HAL\Debug(FreeRTOS)" -L"C:\LEETC\SE\MCUXpressoIDE_11.10.0_3148\workspace\NSP\FreeRTOS_DRIVERS\Debug" -L"C:\LEETC\SE\MCUXpressoIDE_11.10.0_3148\workspace\NSP\HAL" -L"C:\LEETC\SE\MCUXpressoIDE_11.10.0_3148\workspace\NSP\CMSIS_CORE_LPC17xx\Debug" -L"C:\LEETC\SE\MCUXpressoIDE_11.10.0_3148\workspace\NSP\DRIVERS\Debug" -L"C:\LEETC\SE\MCUXpressoIDE_11.10.0_3148\workspace\NSP\FreeRTOS-Kernel\Debug" -Xlinker -Map="FreeRTOS-Template.map" -Xlinker --cref -Xlinker --gc-sections -Xlinker -print-memory-usage -mcpu=cortex-m3 -mthumb -T FreeRTOS-Template_Debug.ld -o "FreeRTOS-Template.axf" $(OBJS) $(USER_OBJS) $(LIBS) -lFreeRTOS_DRIVERS -lDRIVERS -lHAL -lFreeRTOS-Kernel -lHALRunTime -lCMSIS_CORE_LPC17xx
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) FreeRTOS-Template.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "FreeRTOS-Template.axf"; # arm-none-eabi-objcopy -v -O binary "FreeRTOS-Template.axf" "FreeRTOS-Template.bin" ; # checksum -p LPC1769 -d "FreeRTOS-Template.bin";
	-@echo ' '

.PHONY: all clean dependents main-build post-build

-include ../makefile.targets
