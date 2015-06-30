# **************************** Select Task Solution ****************************
C_TARGET = hello_OLED.c
# C_TARGET = hello_graphics.c
# C_TARGET = hello_logo.c
CLOCK = 16000000
# ******************************************************************************

# **************************** Device & Programmer ****************************
DEVICE = atmega328p
PROGRAMMER = -c avrisp2 -P /dev/tty.usbmodem00065771
# ******************************************************************************

# ****************************** Short cut macros ******************************
AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE) -U flash:w:main.hex:i
# ******************************************************************************

INCLUDES += u8g_clip.c
INCLUDES += u8g_com_api.c
INCLUDES += u8g_com_atmega_hw_spi.c
INCLUDES += u8g_com_io.c
INCLUDES += u8g_delay.c
INCLUDES += u8g_dev_ssd1306_128x64.c
INCLUDES += u8g_font.c
INCLUDES += u8g_font_data.c
INCLUDES += u8g_ll_api.c
INCLUDES += u8g_page.c
INCLUDES += u8g_pb.c
INCLUDES += u8g_pb8v1.c
INCLUDES += u8g_state.c

COMMON_FLAGS += -g -Os -Wall -funsigned-char -funsigned-bitfields
COMMON_FLAGS += -ffunction-sections -fdata-sections
COMMON_FLAGS += -Wl,--gc-sections
COMMON_FLAGS += -Wl,--relax -mcall-prologues
CFLAGS = $(COMMON_FLAGS) -std=gnu99 -Wstrict-prototypes


all: build

build:
	# Compiling C with avr-gcc to main.elf
	@avr-gcc $(CFLAGS) -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) $(INCLUDES) $(C_TARGET) -o main.elf

	# Converting to .hex
	@avr-objcopy -O ihex main.elf main.hex

	# Information about the flash.hex
	@avr-size --format=avr --mcu=$(DEVICE) main.hex

	# Flash C $(C_TARGET).hex to chip
	$(AVRDUDE)

clean:
	@rm -f *.cof *.hex *.obj *.elf
