# **** Update for the AVR device, the Programmer and the clock of this chip ****
DEVICE = atmega328p
CLOCK = 16000000
PROGRAMMER = -c avrisp2 -P /dev/tty.usbmodem00065771
# ******************************************************************************

all:
	# =====>>>>> Building Target...
	avr-gcc -Wall -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) main.c -o main.elf
	# =====>>>>> main.elf built successfully!
	#
	# =====>>>>> Building main.hex...
	avr-objcopy -O ihex main.elf main.hex
	# =====>>>>> main.hex ready for upload!

clean:
	#
	# Cleaning...
	@rm -f main.hex main.elf
	#...done.

flash:
	#
	# =====>>>>> Flashing onto Device
	avrdude $(PROGRAMMER) -p $(DEVICE) -U flash:w:main.hex:i
	# =====>>>>> Done!!
