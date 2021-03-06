
# User defined values
TARGETNAME = u8m2
MCU:=atmega328p
F_CPU:=16000000
WORKDIR:=.
U8GM2DIR:=../src

AVRDUDE_PROGRAMMER := avrisp2
# AVRDUDE_PROGRAMMER := usbasp

# AVRDUDE_PORT := usb
AVRDUDE_PORT := /dev/tty.usbmodem00065771
# Replace standard build tools by avr tools
CC = avr-gcc
AR  = @avr-ar

# Compile all .c files in all directories
SRC = $(shell ls $(WORKDIR)/*.c 2>/dev/null)
SRC += $(shell ls $(U8GM2DIR)/*.c 2>/dev/null)

# Flags for the linker and the compiler
COMMON_FLAGS = -DF_CPU=$(F_CPU) -mmcu=$(MCU)
COMMON_FLAGS += -I$(WORKDIR) -I$(U8GM2DIR)
COMMON_FLAGS += -g -Os -Wall -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
COMMON_FLAGS += -ffunction-sections -fdata-sections -Wl,--gc-sections
COMMON_FLAGS += -Wl,--relax -mcall-prologues
CFLAGS = $(COMMON_FLAGS) -std=gnu99 -Wstrict-prototypes

OBJ = $(SRC:.c=.o)

.SUFFIXES: .elf .hex .dis

# Targets
all: $(TARGETNAME).dis $(TARGETNAME).hex
	avr-size $(TARGETNAME).elf

upload: $(TARGETNAME).dis $(TARGETNAME).hex
	avrdude -F -p $(MCU) -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER) -v -v -U flash:w:$(TARGETNAME).hex
	avr-size $(TARGETNAME).elf

clean:
	rm -f $(TARGETNAME).hex $(TARGETNAME).elf $(TARGETNAME).a $(TARGETNAME).dis $(OBJ)

# implicit rules
.elf.hex:
	avr-objcopy -O ihex -R .eeprom $< $@

# explicit rules
$(TARGETNAME).elf: $(TARGETNAME).a($(OBJ))
	$(LINK.o) $(COMMON_FLAGS) $(TARGETNAME).a $(LOADLIBES) $(LDLIBS) -o $@

$(TARGETNAME).dis: $(TARGETNAME).elf
	avr-objdump -S $< > $@

