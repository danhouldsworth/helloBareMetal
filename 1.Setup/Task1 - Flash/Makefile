# *********************** Update for the Programmer used ***********************
PROGRAMMER = -c avrisp2 -P /dev/tty.usbmodem00065771
# ******************************************************************************

all:
	# =====>>>>> Building Target...
	avr-gcc -Wall -mmcu=atmega328p main.c -o main.elf
	# =====>>>>> main.elf built successfully!
	#
	# =====>>>>> Building main.hex...
	avr-objcopy -O ihex main.elf main.hex
	# =====>>>>> main.hex ready for upload!
	#
	# =====>>>>> Flashing onto Device
	avrdude $(PROGRAMMER) -p atmega328p -U flash:w:main.hex:i
	# =====>>>>> Done!!

