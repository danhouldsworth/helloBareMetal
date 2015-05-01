# **************************** Device & Programmer ****************************
DEVICE = atmega328
CLOCK = 8000000

# PROGRAMMER = -c avrisp2 -P /dev/tty.usbmodem00065771
PROGRAMMER = -c usbasp -P usb
# ******************************************************************************

all: build

build:
	avr-gcc -Wall -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) main4.c -o main4.elf
	avr-objcopy -O ihex main4.elf main4.hex
	avr-size --format=avr --mcu=$(DEVICE) main4.hex
	avrdude $(PROGRAMMER) -p $(DEVICE) -U flash:w:main4.hex:i

asm:
	avra -fI main1.asm
	avrdude $(PROGRAMMER) -p $(DEVICE) -U flash:w:main1.hex:i

clean:
	@rm -f *.cof *.hex *.obj *.elf
