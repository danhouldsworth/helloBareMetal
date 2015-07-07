# **************************** Select Task Solution ****************************
C_TARGET = photoPan3.c
CLOCK = 16000000
# CLOCK = 8000000
# ******************************************************************************

# **************************** Device & Programmer ****************************
DEVICE = atmega328
# PROGRAMMER = -c usbasp -P usb
PROGRAMMER = -c avrisp2 -P /dev/tty.usbmodem00065771
# ******************************************************************************

# ****************************** Short cut macros ******************************
AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE)
# ******************************************************************************

CFLAGS = -Os -Wall -std=gnu99

all: build

build:
	# Compiling $(C_TARGET) with avr-gcc to main.elf
	@avr-gcc $(CFLAGS) -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) $(INCLUDES) $(C_TARGET) -o main.elf

	# Converting to .hex
	@avr-objcopy -O ihex main.elf main.hex

	# Information about the flash.hex
	@avr-size --format=avr --mcu=$(DEVICE) main.hex

	# Set Fuses for Internal 16MHz RC (NANO V4)
	# $(AVRDUDE) -U hfuse:w:0xda:m -U lfuse:w:0xff:m -U efuse:w:0x05:m
	# Set Fuses for Internal 16MHz RC
	$(AVRDUDE) -U hfuse:w:0xde:m -U lfuse:w:0xff:m -U efuse:w:0x05:m
	# Set Fuses for Internal 8MHz RC
	# $(AVRDUDE) -U hfuse:w:0xde:m -U lfuse:w:0xe2:m -U efuse:w:0x05:m

	# Flash C $(C_TARGET).hex to chip
	$(AVRDUDE) -U flash:w:main.hex:i

clean:
	@rm -f *.cof *.hex *.obj *.elf
