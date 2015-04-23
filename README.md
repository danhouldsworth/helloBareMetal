# helloBareMetal
A follow on from helloLanguages, dedicated to bare metal programming of embedded devices (Atmel AVR &amp; ARM Cortex)

## TBC - whether to include RasberryPi?
On the one hand most of the challenges of helloLanguages could be tackled on a RasberryPi running Rasbian. However, when compiling and running assembler, we would have to do it on the machine (rather than develope on an Intel Mac for instance).
On the other hand, if we scrapped the Linux OS, we have a great piece of physical kit with loads of GPIO and connected peripherals (LEDs / GPU / USB / Camera) to experiment with directly from bare metal programming. eg see [this tutorial](http://www.valvers.com/open-software/raspberry-pi/step01-bare-metal-programming-in-cpt1/) to blink the RaspberryPI LEDs.

# Wishlist (AVR / ARM)
* Blink LED
* Software timers (based on NOP and known Clock)
* Hardware Timers
* Concurrency
* Schedular
* Serial Port - Hello, world!

# Build environments

### AVR 8-bit

		brew install avrdude
		brew tap larsimmisch/avr
		brew install avr-libc // avr-gcc is a dependancy of avr-libc

### ARM-cortex

		brew tap PX4/homebrew-px4
		brew install gcc-arm-none-eabi


# Questions

* Understand how ```avrdude```, ```avr-gcc``` and ```avr-libc``` differ and depend.
* Is the build parameter ```avrdude --with-usb``` actually doing anything? Doesn't seem to be used in the brew formula, unless being passed to dependancies?
