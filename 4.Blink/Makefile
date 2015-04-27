# **************************** Device & Programmer ****************************
DEVICE = atmega328p
PROGRAMMER = -c avrisp2 -P /dev/tty.usbmodem00065771
# ******************************************************************************

asm:
	avra -fI main.asm

flash:
	avrdude $(PROGRAMMER) -p $(DEVICE) -U flash:w:main.hex:i

clean:
	@rm -f *.cof *.hex *.obj
