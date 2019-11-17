################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include example/src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: Lab1_Wireless_Communication.axf

# Tool invocations
Lab1_Wireless_Communication.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"/Users/sanjayjain/Documents/MCUXpressoIDE_11.0.0/workspace_240/lpc_chip_175x_6x/Debug" -L"/Users/sanjayjain/Documents/MCUXpressoIDE_11.0.0/workspace_240/lpc_board_nxp_lpcxpresso_1769/Debug" -Xlinker -Map="Lab1_Wireless_Communication.map" -Xlinker --gc-sections -mcpu=cortex-m3 -mthumb -T "Lab1_Wireless_Communication_Debug.ld" -o "Lab1_Wireless_Communication.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) Lab1_Wireless_Communication.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "Lab1_Wireless_Communication.axf"; # arm-none-eabi-objcopy -v -O binary "Lab1_Wireless_Communication.axf" "Lab1_Wireless_Communication.bin" ; # checksum -p LPC1769 -d "Lab1_Wireless_Communication.bin";
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
