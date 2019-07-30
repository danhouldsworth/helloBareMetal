# **************************** Select Task Solution ****************************
ASM_TARGET = main_1Hz.asm
# C_TARGET = main.c
C_TARGET = main.c

# C_CLOCK = 8000000
C_CLOCK = 16000000
# ******************************************************************************

# **************************** Device & Programmer ****************************
# DEVICE = t85
# DEVICE = atmega328p
DEVICE = atmega32u4
PROGRAMMER = -c usbasp -P usb
# PROGRAMMER = -c avrisp2 -P /dev/tty.usbmodem00065771
# ******************************************************************************

# ****************************** Short cut macros ******************************
AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE)
# ******************************************************************************

all: build

build:
	# Compiling C with avr-gcc to main.elf
	@avr-gcc -Wall -DF_CPU=$(C_CLOCK) -mmcu=$(DEVICE) $(C_TARGET) -o main.elf

	# Converting to .hex
	@avr-objcopy -O ihex main.elf main.hex

	# Information about the flash.hex
	@avr-size --format=avr --mcu=$(DEVICE) main.hex

	# Flash C .hex to chip
	$(AVRDUDE) -U flash:w:main.hex:i

asm:
	# Compiling ASM with avra directly to .hex
	avra -fI $(ASM_TARGET)

	# Flash ASM .hex to chip
	$(AVRDUDE) -U flash:w:$(ASM_TARGET:.asm=.hex):i

clean:
	@rm -f *.cof *.hex *.obj *.elf


# ATtiny85 Fuses
HIGH_FUSE = 0xdE
LOW_FUSE = 0x62
EXT_FUSE = 0xff
