SHELL		:= /bin/bash

all:
	avr-gcc -Wall -Os -DF_CPU=16000000UL -mmcu=atmega328p serial.c -o serial.elf
	avr-objcopy -O ihex -R .eeprom serial.elf serial.hex

show: 
	qemu-system-avr -machine uno -bios serial.elf -nographic -serial tcp::5678,server=on,wait=on

print:
	qemu-system-avr -machine uno -bios serial.elf -d 'guest_errors' -nographic -serial tcp::5678,server=on,wait=on

net:
	telnet localhost 5678 | tee out
