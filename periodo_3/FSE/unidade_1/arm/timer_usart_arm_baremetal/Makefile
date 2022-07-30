SHELL		:= /bin/bash
PROJECT_NAME    := timer_usart_arm

all:
	timeout --preserve-status 60s qemu-system-arm -icount shift=auto,align=on -M netduino2 -nographic -monitor null -serial stdio -kernel $(PROJECT_NAME).bin -d int < <(while read -r line; do word=($$line); sleep $${word[0]}; echo $${word[1]}; done < arm.input) > arm.output 2>&1
	awk -i inplace '/^([0-9]+|B|S|R|Y|G)/ { print $$0 } /taking.*exception (44|45|46|66)/ { print "timer_interruption" } /taking.*exception (53|54|55|68|69)/ { print "usart_interruption" }' arm.output
print:
	qemu-system-arm -machine netduino2 -kernel $(PROJECT_NAME).bin -d int -nographic -monitor null -serial stdio

show:
	qemu-system-arm -machine netduino2 -kernel $(PROJECT_NAME).bin -d 'in_asm,int,exec,cpu,guest_errors,unimp' -nographic -serial tcp::5678,server=on,wait=on

debug:
	qemu-system-arm -machine netduinoplus2 -s -S -kernel $(PROJECT_NAME).bin -d 'guest_errors' -nographic -monitor null -serial stdio

