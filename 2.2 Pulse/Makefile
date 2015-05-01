# **************************** Device & Programmer ****************************
DEVICE = atmega328
CLOCK = 8000000
PROGRAMMER = -c usbasp -P usb

HIGH_FUSE = 0xdE
LOW_FUSE = 0xe2
EXT_FUSE = 0x05
# ******************************************************************************

all: build

build:
	avr-gcc -Wall -std=c99 -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) main.c -o main.elf
	avr-objcopy -O ihex main.elf main.hex
	avr-size --format=avr --mcu=$(DEVICE) main.hex
	avrdude $(PROGRAMMER) -p $(DEVICE) -U flash:w:main.hex:i

asm:
	avra -fI main.asm
	avrdude $(PROGRAMMER) -p $(DEVICE) -U flash:w:main.hex:i

setfuse:
	avrdude $(PROGRAMMER) -p $(DEVICE) -U hfuse:w:$(HIGH_FUSE):m -U lfuse:w:$(LOW_FUSE):m -U efuse:w:$(EXT_FUSE):m

clean:
	@rm -f *.cof *.hex *.obj *.elf
