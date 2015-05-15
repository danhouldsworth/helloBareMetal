# **************************** Select Task Solution ****************************
ASM_TARGET = main.asm
C_TARGET = main.c

CLOCK = 16000000
# ******************************************************************************

# **************************** Device & Programmer ****************************
DEVICE = atmega328p
# PROGRAMMER = -c usbasp -P usb
PROGRAMMER = -c avrisp2 -P /dev/tty.usbmodem00065771
# ******************************************************************************

# ****************************** Short cut macros ******************************
AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE) -U flash:w:main.hex:i
# ******************************************************************************

all: build

build:
	# Compiling C with avr-gcc to main.elf
	@avr-gcc -std=c99 -Wall -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) $(C_TARGET) -o main.elf

	# Converting to .hex
	@avr-objcopy -O ihex main.elf main.hex

	# Information about the flash.hex
	@avr-size --format=avr --mcu=$(DEVICE) main.hex

	# Flash C .hex to chip
	$(AVRDUDE)

asm:
	# Compiling ASM with avra directly to .hex
	avra -DF_CPU=$(CLOCK) --includepath ../../Reference/Libs -fI $(ASM_TARGET) -o main.hex

	# Flash ASM .hex to chip
	$(AVRDUDE)

clean:
	@rm -f *.cof *.hex *.obj *.elf
