# **************************** Select Task Solution ****************************
C_TARGET = bonus.c
C_TARGET = ESCproject.c

CLOCK = 16000000
# ******************************************************************************

# **************************** Device & Programmer ****************************
DEVICE = atmega328
DEVICE = atmega328p
PROGRAMMER = -c usbasp -P usb
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

	# Flash C $(C_TARGET).hex to chip
	$(AVRDUDE)

clean:
	@rm -f *.cof *.hex *.obj *.elf
