# Task

1. Write a Makefile that compiles and flashes a trivial (empty loop) program onto a target MCU.
2. Extend it to have ```make clean``` and ```make flash``` capabilities.
3. Extend it to enable the setting of the device MCU fuses, and reading of the device ROM, EEPROM & fuses.
4. Set the fuses such that it needs no external oscillator.

## Bonus Challenge:

* What is the simplest ISP / Chip / LED board we can make for convenience of learning.
* Sacrifice a chip by setting the fuse lock bit, and demonstrating we're now locked out ;-)
* Upload onto a chip pre-loaded with the optiboot bootloader

## Learning questions :

1. What does .phony do in a Makefile? Do we need it? How would we use it?
2. What does avr-ar do? What is the difference between linking a file.o at compile time vs linking a core library?

## Notes:

* You can use ```make --makefile MakefileX``` to specify which Makefile to use from these solutions
eg. ```make --makefile Makefile3 setfuse flash read clean``` to show solution to 3.
* See http://eleccelerator.com/fusecalc/fusecalc.php?chip=atmega328p for fuse settings
