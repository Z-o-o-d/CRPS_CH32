################################################################################
# MRS Version: 1.9.1
# 自动生成的文件。不要编辑！
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include User/subdir.mk
-include Startup/subdir.mk
-include Peripheral/src/subdir.mk
-include Debug/subdir.mk
-include Core/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
CRPS.hex \

SECONDARY_LIST += \
CRPS.lst \

SECONDARY_SIZE += \
CRPS.siz \


# 所有目标
all: CRPS.elf secondary-outputs

# 工具调用
CRPS.elf: $(OBJS) $(USER_OBJS)
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -T "C:\Users\87407\OneDrive\EMBcompCRPS_CH32\prj\CRPS\Ld\Link.ld" -nostartfiles -Xlinker --gc-sections -Wl,-Map,"CRPS.map" --specs=nano.specs --specs=nosys.specs -o "CRPS.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@	@
CRPS.hex: CRPS.elf
	@	riscv-none-embed-objcopy -O ihex "CRPS.elf"  "CRPS.hex"
	@	@
CRPS.lst: CRPS.elf
	@	riscv-none-embed-objdump --all-headers --demangle --disassemble -M xw "CRPS.elf" > "CRPS.lst"
	@	@
CRPS.siz: CRPS.elf
	@	riscv-none-embed-size --format=berkeley "CRPS.elf"
	@	@
# 其他目标
clean:
	-$(RM) $(ASM_UPPER_DEPS)$(OBJS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS) CRPS.elf
	-@
secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets
