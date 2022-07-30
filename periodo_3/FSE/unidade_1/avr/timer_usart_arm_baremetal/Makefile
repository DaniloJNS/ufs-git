SHELL		:= /bin/bash
PROJECT_NAME    := timer_usart_arm

all:
	timeout --preserve-status 60s qemu-system-arm -icount shift=auto,align=on -M netduino2 -nographic -monitor null -serial stdio -kernel $(PROJECT_NAME).bin -d int < <(while read -r line; do word=($$line); sleep $${word[0]}; echo $${word[1]}; done < arm.input) > arm.output 2>&1
	awk -i inplace '/^([0-9]+|B|S|R|Y|G)/ { print $$0 } /taking.*exception (44|45|46|66)/ { print "timer_interruption" } /taking.*exception (53|54|55|68|69)/ { print "usart_interruption" }' arm.output
