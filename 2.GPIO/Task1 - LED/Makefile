# **************************** Select Task Solution ****************************
ASM_TARGET = main_328p.asm
# C_TARGET = main.c
C_TARGET = bonus_328p.c
# ******************************************************************************

# **************************** Device & Programmer ****************************
DEVICE = atmega328
PROGRAMMER = -c usbasp -P usb
# ******************************************************************************

# ****************************** Short cut macros ******************************
AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE)
# ******************************************************************************

all: build

build:
	# Compiling C with avr-gcc to main.elf
	@avr-gcc -Wall -mmcu=$(DEVICE) $(C_TARGET) -o main.elf

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

