ARM_PREFIX      := arm-none-eabi
ARM_AS          := $(ARM_PREFIX)-as
ARM_CC          := $(ARM_PREFIX)-gcc
ARM_LD          := $(ARM_PREFIX)-ld
ARM_OC          := $(ARM_PREFIX)-objcopy
ARM_AS_OPTIONS	:= -mcpu=cortex-m3 -mthumb -g
ARM_CC_OPTIONS	:= -Wall -Os -mcpu=cortex-m3 -mthumb -g -nostdlib -nostartfiles
C_SOURCES       := blink_button.c
S_SOURCES       :=
PROJECT_NAME    := blink_button

all: $(PROJECT_NAME).bin
clean:
	@rm -rf *.o *.elf *.bin
download: $(PROJECT_NAME).bin
	st-flash write $(PROJECT_NAME).bin 0x08000000
%.o: %.s
	$(ARM_AS) $(ARM_AS_OPTIONS) $(<) -o $(@)
%.o: %.c
	$(ARM_CC) $(ARM_CC_OPTIONS) -c $(<) -o $(@)
$(PROJECT_NAME).elf: stm32f103c8.ld $(S_SOURCES:.s=.o) $(C_SOURCES:.c=.o)
	$(ARM_CC) $(ARM_CC_OPTIONS) -T $(^) -o $(@)
$(PROJECT_NAME).bin: $(PROJECT_NAME).elf
	$(ARM_OC) -O binary $(<) $(@)

print:
	qemu-system-arm -machine netduino2 -kernel $(PROJECT_NAME).bin -d 'in_asm,int,exec,cpu,guest_errors,unimp' -nographic -serial tcp::5678,server=on,wait=on
