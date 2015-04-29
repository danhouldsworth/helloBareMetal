# **************************** Device & Programmer ****************************
DEVICE = atmega328
CLOCK = 8000000

PROGRAMMER = -c avrisp2 -P /dev/tty.usbmodem00065771
# ******************************************************************************

all: build

build:
	avr-gcc -Wall -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) main4.c -o main4.elf
	avr-objcopy -O ihex main4.elf main4.hex
	avr-size --format=avr --mcu=$(DEVICE) main4.hex
	avrdude $(PROGRAMMER) -p $(DEVICE) -U flash:w:main4.hex:i

asm:
	avra -fI main3.asm
	avrdude $(PROGRAMMER) -p $(DEVICE) -U flash:w:main3.hex:i

clean:
	@rm -f *.cof *.hex *.obj *.elf
