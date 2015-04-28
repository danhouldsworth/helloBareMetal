# **************************** Device & Programmer ****************************
DEVICE = atmega328
CLOCK = 8000000

PROGRAMMER = -c avrisp2 -P /dev/tty.usbmodem00065771
# ******************************************************************************

all: build

build:
	avr-gcc -Wall -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) main3.c -o main3.elf
	avr-objcopy -O ihex main3.elf main3.hex
	avr-size --format=avr --mcu=$(DEVICE) main3.hex
	avrdude $(PROGRAMMER) -p $(DEVICE) -U flash:w:main3.hex:i

asm:
	avra -fI main.asm
	avrdude $(PROGRAMMER) -p $(DEVICE) -U flash:w:main.hex:i

clean:
	@rm -f *.cof *.hex *.obj *.elf
