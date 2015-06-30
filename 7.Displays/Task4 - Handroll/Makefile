# **************************** Select Task Solution ****************************
# ASM_TARGET = checkers.asm
ASM_TARGET = lines.asm
# ******************************************************************************

# **************************** Device & Programmer ****************************
PROGRAMMER = -c avrisp2 -P /dev/tty.usbmodem00065771
# ******************************************************************************

# ****************************** Short cut macros ******************************
AVRDUDE = avrdude $(PROGRAMMER) -p atmega328p
# ******************************************************************************

asm:
	# Compiling ASM with avra directly to .hex
	avra --includepath ../../Reference/Libs/defSheets -fI $(ASM_TARGET)

	# Flash ASM .hex to chip
	$(AVRDUDE) -U flash:w:$(ASM_TARGET:.asm=.hex):i

clean:
	@rm -f *.cof *.hex *.obj *.elf
