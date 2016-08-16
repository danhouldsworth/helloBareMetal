# **************************** Select Task Solution ****************************
C_TARGET = internal.c
CLOCK = 8000000
# ******************************************************************************

# **************************** Device & Programmer ****************************
DEVICE = atmega328p
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
	@avr-gcc $(CFLAGS) -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) $(C_TARGET) -o main.elf

	# Converting to .hex
	@avr-objcopy -O ihex main.elf main.hex

	# Information about the flash.hex
	@avr-size --format=avr --mcu=$(DEVICE) main.hex

	# Set eFuse for default disable BOD
	# $(AVRDUDE) -U efuse:w:0x07:m
	# Set hFuse for Enable SPIprogramming, min BootSector and disable RST-->BOOT
	# $(AVRDUDE) -U hfuse:w:0xdf:m
	# Set lFuse for internal 8MHz clk / 1
	# $(AVRDUDE) -U lfuse:w:0xe2:m
	# Set lFuse for Ext Crystal Oscillator / max start up / clk / 1
	# $(AVRDUDE) -U lfuse:w:0xf7:m

	# Flash C $(C_TARGET).hex to chip
	$(AVRDUDE) -U flash:w:main.hex:i

clean:
	@rm -f *.cof *.hex *.obj *.elf
