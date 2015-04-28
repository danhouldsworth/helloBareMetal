# helloBareMetal
A follow on from [helloLanguages](https://github.com/danhouldsworth/helloLanguages), dedicated to bare metal programming of embedded devices (Atmel AVR &amp; ARM Cortex)

# Background
While I've been programming various AVRs for drone building (both Flight Controllers and Electronic Speed Controllers), and also back when tinkering with Arduino, I've come unstuck with various projects in a way that makes me realise I don't have a deep understanding of the underlying principles of embedded MCUs.
I'm left with questions like :
* What does it really  mean for a variable to be volatile or static?
* What is actually going on when we register an Interupt Service Routine?
* Do I really know which section of memory my program is sitting, vs my data, vs the PORT registers, vs the bootloader?
* Am I free to compile optimised for speed instead of size?

Additionally, while big projects go slowly in Assembler, I always feel I learn more deeply what is actually going on, and so would like to tackle like for like projects in both C and assembler.

# Arduino
This deserves a special mention, as while I've learned a lot at first from working through Arduino projects, and also benefited from the compatabitliy as everyone makes drives and libraries for it. I find the IDE a pig, that slows down development of all but the most trivial project, and blocks me from actually understanding what is going on when we compile or flash a project.

# ARM-cortex
This also deserves a special mention, as ultimately my target projects will inevitabilty take me in the direction of a 32-bit CPU with built in float support. But - and it's a big but - as I've tried to move over to the ARM-cortex, I realise I'm even more in the dark on all the issues raised above. Getting just the Blink program to load onto the Teensy3.1 ARM-CortexM4 seemed to hard, without using the Teensduino plugin for Arduino IDE. And so this has kickstarted my desire to get back to first principles and truely understand what's going on under the hood. Using nothing more than a text editor, and simple command line tools that I will install myself. I am then anticipating, that a deep knowledge of the 8-bit AVR MCUs will then speed my transition into the 32-bit ARM world.

# Operating Systems
Approaching from a further angle still. Another motive for this project is my desire to understand more about what is actually going on with an operating system and a schedular. And while [helloLanguages](https://github.com/danhouldsworth/helloLanguages) is a fascinating exploration of different langauges, they are all sitting on top of an operating system which is abstracting away the hardware. What better way to learn about operating systems than to get of them completely, and then build back their services as and when needed - eg. multi tasking, scheduling etc

## TBC - whether to include RasberryPi?
On the one hand most of the challenges of helloLanguages could be tackled on a RasberryPi running Rasbian. However, when compiling and running assembler, we would have to do it on the machine (rather than develop on an Intel Mac for instance).
On the other hand, if we scrapped the Linux OS, we have a great piece of physical kit with loads of GPIO and connected peripherals (LEDs / GPU / USB / Camera) to experiment with directly from bare metal programming. eg see [this tutorial](http://www.valvers.com/open-software/raspberry-pi/step01-bare-metal-programming-in-cpt1/) to blink the RaspberryPI LEDs.


# Build environments

### AVR 8-bit

		brew install avrdude avra
		brew tap larsimmisch/homebrew-avr
		brew install avr-libc // avr-gcc is a dependancy of avr-libc

### ARM-cortex

		brew tap PX4/homebrew-px4
		brew install gcc-arm-none-eabi


# Questions / Miscallaneous

* Understand how ```avrdude```, ```avr-gcc``` and ```avr-libc``` differ and depend.
* Is the build parameter ```avrdude --with-usb``` actually doing anything? Doesn't seem to be used in the brew formula, unless being passed to dependancies?
* What does it mean for Yosemite if there's only formula specified for Mavericks, Mountain Lion etc.?

# Developer environment quirks / notes

* Install Silcone Labs driver (dongle straight to pins / jumpers) : Install SiliconLabs CP210x Macintosh OSX VCP Driver v3.1 or from my BitBucket
==> Needed for USB / PWM ESC programmer
* Mac OSX Yosemite not recognising atmega32u4 /dev/tty.usbmodemxxxxx - need to reset by shorting RST pin to ground which enables it for a few seconds window.
