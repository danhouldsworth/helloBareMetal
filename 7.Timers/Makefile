# **************************** Device & Programmer ****************************
DEVICE = atmega328p
DEVICE = m32u4
CLOCK = 8000000

HIGH_FUSE = 0xdE
LOW_FUSE = 0xde
EXT_FUSE = 0x05

# PROGRAMMER = -c avrisp2 -P /dev/tty.usbmodem00065771
PROGRAMMER = -c usbasp -P usb
PROGRAMMER = -c avr109 -P /dev/tty.usbserial
PROGRAMMER = -c avr109 -P /dev/tty.usbmodem1421
# ******************************************************************************

all: build

build:
	avr-gcc -Wall -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) main.c -o main.elf
	avr-objcopy -O ihex main.elf main.hex
	avr-size --format=avr --mcu=$(DEVICE) main.hex
	avrdude $(PROGRAMMER) -p $(DEVICE) -U flash:w:main.hex:i

asm:
	avra -fI main.asm
	avrdude $(PROGRAMMER) -p $(DEVICE) -v -v -v -U flash:w:main.hex:i

setfuse:
	avrdude $(PROGRAMMER) -p $(DEVICE) -U hfuse:w:$(HIGH_FUSE):m -U lfuse:w:$(LOW_FUSE):m -U efuse:w:$(EXT_FUSE):m

clean:
	@rm -f *.cof *.hex *.obj *.elf
