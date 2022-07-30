SHELL			:= /bin/bash
PROJECT_NAME    := led

all:
	arm-none-eabi-as -mcpu=cortex-m3 -mthumb -g start.s -o start.o
	arm-none-eabi-gcc -Wall -Os -mcpu=cortex-m3 -mthumb -g -nostdlib -nostartfiles -c $(PROJECT_NAME).c -o $(PROJECT_NAME).o
	arm-none-eabi-gcc -Wall -Os -mcpu=cortex-m3 -mthumb -g -nostdlib -nostartfiles -T stm32f205.ld start.o $(PROJECT_NAME).o -o $(PROJECT_NAME).elf
	arm-none-eabi-objcopy -O binary $(PROJECT_NAME).elf $(PROJECT_NAME).bin

main:
	timeout --preserve-status 60s qemu-system-arm -icount shift=auto,align=on -M netduino2 -nographic -monitor null -serial stdio -kernel $(PROJECT_NAME).bin -d int < <(while read -r line; do word=($$line); sleep $${word[0]}; echo $${word[1]}; done < arm.input) > arm.output 2>&1
	awk -i inplace '/^([0-9]+|B|S|R|Y|G)/ { print $$0 } /taking.*exception (44|45|46|66)/ { print "timer_interruption" } /taking.*exception (53|54|55|68|69)/ { print "usart_interruption" }' arm.output

show: 
	timeout --preserve-status 60s qemu-system-arm -icount shift=auto,align=on -M netduino2 -nographic -monitor null -serial stdio -kernel $(PROJECT_NAME).bin -d int 

print:
	qemu-system-arm -machine netduino2 -kernel $(PROJECT_NAME).bin -d int -nographic -monitor null -serial stdio

debug:
	qemu-system-arm -machine netduino2 -gdb tcp::8000 -S -kernel $(PROJECT_NAME).bin -d 'guest_errors' -nographic -monitor null -serial stdio
