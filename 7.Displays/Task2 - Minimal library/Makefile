# **************************** Select Task Solution ****************************
C_TARGET = particle_OLED.c
CLOCK = 16000000
# ******************************************************************************

# **************************** Device & Programmer ****************************
DEVICE = atmega328p
PROGRAMMER = -c avrisp2 -P /dev/tty.usbmodem00065771
# ******************************************************************************

# ****************************** Short cut macros ******************************
AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE) -U flash:w:main.hex:i
# ******************************************************************************

# INCLUDES += OLED.c
# INCLUDES += SSD1306.h

COMMON_FLAGS += -g -Os -Wall -funsigned-char
# COMMON_FLAGS += -mno-short-calls -nodefaultlibs -lm -lgcc -lc -lgcc
# COMMON_FLAGS += -ffunction-sections -fdata-sections
# COMMON_FLAGS += -Wl,--gc-sections
# COMMON_FLAGS += -Wl,--relax -mcall-prologues
CFLAGS = $(COMMON_FLAGS) -std=gnu99

all: build

build:
	# Compiling C with avr-gcc to main.elf
	# @avr-gcc $(CFLAGS) -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) $(INCLUDES) $(C_TARGET) -o main.elf
	@avr-gcc -lc -lm $(CFLAGS) -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) $(INCLUDES) $(C_TARGET) -o main.elf -lc

	# Converting to .hex
	@avr-objcopy -O ihex main.elf main.hex

	# Information about the flash.hex
	@avr-size --format=avr --mcu=$(DEVICE) main.hex

	# Flash C $(C_TARGET).hex to chip
	$(AVRDUDE)

clean:
	@rm -f *.cof *.hex *.obj *.elf
