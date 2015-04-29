# **************************** Device & Programmer ****************************
DEVICE = atmega328p
DEVICE = atmega328
CLOCK = 8000000
PROGRAMMER = -c avrisp2 -P /dev/tty.usbmodem00065771
# ******************************************************************************

all: build

build:
	avr-gcc -Wall -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) main.c -o main.elf
	avr-objcopy -O ihex main.elf main.hex
	avr-size --format=avr --mcu=$(DEVICE) main.hex
	avrdude $(PROGRAMMER) -p $(DEVICE) -U flash:w:main.hex:i

asm:
	avra -fI main.asm
	avrdude $(PROGRAMMER) -p $(DEVICE) -U flash:w:main.hex:i

asm2:
	avra -fI main2.asm
	avrdude $(PROGRAMMER) -p $(DEVICE) -U flash:w:main2.hex:i

clean:
	@rm -f *.cof *.hex *.obj *.elf
