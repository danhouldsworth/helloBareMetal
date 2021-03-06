# **************************** Device & Programmer ****************************
# DEVICE = atmega328p
DEVICE = m32u4

HIGH_FUSE = 0xdE
LOW_FUSE = 0xff
EXT_FUSE = 0x05

# PROGRAMMER = -c avrisp2 -P /dev/tty.usbmodem00065771
PROGRAMMER = -c usbasp -P usb
# ******************************************************************************

# ****************************** Short cut macros ******************************
AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE)
# ******************************************************************************

setfuse:
	$(AVRDUDE) -U hfuse:w:$(HIGH_FUSE):m -U lfuse:w:$(LOW_FUSE):m -U efuse:w:$(EXT_FUSE):m

read:
	$(AVRDUDE) -U flash:r:flash.hex:i -U eeprom:r:eeprom.hex:i -U hfuse:r:hfuse.hex:h -U lfuse:r:lfuse.hex:h -U efuse:r:efuse.hex:h
	cat flash.hex
	cat eeprom.hex
	cat hfuse.hex
	cat efuse.hex
	cat lfuse.hex
	avr-size --format=avr --mcu=$(DEVICE) flash.hex

#---------------------------------------------------------------------
# ATMega328P
#---------------------------------------------------------------------
# Fuse extended byte:
# 0x03 = - - - -   - 0 1 1
#                    \-+-/
#                      +------ BODLEVEL 0..2 (011 = 4.3V)
# Fuse high byte:
# 0xda = 1 1 0 1   1 0 1 0 <-- BOOTRST (0 = jump to bootloader at start)
#        ^ ^ ^ ^   ^ \+/
#        | | | |   |  +------- BOOTSZ 0..1 (01 = 2KB starting at 0x7800)
#        | | | |   + --------- EESAVE (don't preserve EEPROM over chip erase)
#        | | | +-------------- WDTON (1 = watchdog disabled at start)
#        | | +---------------- SPIEN (0 = allow serial programming)
#        | +------------------ DWEN (1 = debug wire disable)
#        +-------------------- RSTDISBL (1 = reset pin is enabled)
# Fuse low byte:
# 0xf7 = 1 1 1 1   0 1 1 1
#        ^ ^ \ /   \--+--/
#        | |  |       +------- CKSEL 3..0 (0111 = external full-swing crystal)
#        | |  +--------------- SUT 1..0 (11 = startup time 16K CK/14K + 65ms)
#        | +------------------ CKOUT (1 = clock output disabled)
#        +-------------------- CKDIV8 (1 = do not divide clock by 8)

###############################################################################
clean:
	#
	# Cleaning...
	@rm -f *.hex
	#...done.
