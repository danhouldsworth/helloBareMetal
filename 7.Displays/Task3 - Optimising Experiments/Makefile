# **************************** Select Task Solution ****************************
C_TARGET = fast850HzOLED.c
# C_TARGET = scroll.c
CLOCK = 16000000
# ******************************************************************************

# **************************** Device & Programmer ****************************
DEVICE = atmega328p
PROGRAMMER = -c avrisp2 -P /dev/tty.usbmodem00065771
# ******************************************************************************

# ****************************** Short cut macros ******************************
AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE) -U flash:w:main.hex:i
# ******************************************************************************

CFLAGS = -Os -Wall -std=gnu99

all: build

build:
	# Compiling $(C_TARGET) with avr-gcc to main.elf
	@avr-gcc $(CFLAGS) -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) $(INCLUDES) -S $(C_TARGET)
	@avr-gcc $(CFLAGS) -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) $(INCLUDES) $(C_TARGET) -o main.elf

	# Converting to .hex
	@avr-objcopy -O ihex main.elf main.hex

	# Information about the flash.hex
	@avr-size --format=avr --mcu=$(DEVICE) main.hex

	# Flash C $(C_TARGET).hex to chip
	$(AVRDUDE)

clean:
	@rm -f *.cof *.hex *.obj *.elf
