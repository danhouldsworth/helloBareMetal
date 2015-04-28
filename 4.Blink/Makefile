# **************************** Device & Programmer ****************************
DEVICE = atmega328p
PROGRAMMER = -c avrisp2 -P /dev/tty.usbmodem00065771
# ******************************************************************************

asm:
	avra -fI main.asm
	avrdude $(PROGRAMMER) -p $(DEVICE) -U flash:w:main.hex:i

asm2:
	avra -fI main2.asm
	avrdude $(PROGRAMMER) -p $(DEVICE) -U flash:w:main2.hex:i

clean:
	@rm -f *.cof *.hex *.obj
