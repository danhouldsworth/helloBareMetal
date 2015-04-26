# **************************** Select Task Solution ****************************
TARGET = main1_StdUseOfAVRlibs.c
TARGET = main1_DirectRegisterAddressing.c
TARGET = main2.c
# ******************************************************************************

# **************************** Device & Programmer ****************************
DEVICE = atmega328p
CLOCK = 16000000

HIGH_FUSE = 0xdE
LOW_FUSE = 0xFF
EXT_FUSE = 0x05

PROGRAMMER = -c avrisp2 -P /dev/tty.usbmodem00065771
# ******************************************************************************

# ****************************** Short cut macros ******************************
AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE)
# ******************************************************************************

all: build

build:
	avr-gcc -Wall -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) $(TARGET) -o main.elf
	avr-objcopy -O ihex main.elf main.hex
	avr-size --format=avr --mcu=$(DEVICE) main.hex

clean:
	@rm -f main.hex main.elf eeprom.hex flash.hex hfuse.hex lfuse.hex efuse.hex

flash:
	$(AVRDUDE) -U flash:w:main.hex:i

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
