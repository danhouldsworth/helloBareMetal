# Task

1. Write a Makefile that compiles and flashes a trivial (empty loop) program onto a target MCU.
2. Extend it to have ```make clean``` and ```make flash``` capabilities.
3. Extend it to enable the setting of the device MCU fuses, and reading of the device ROM, EEPROM & fuses.


## Bonus learning :

1. What does .phony do in a Makefile? Do we need it? How would we use it?
2. What does avr-objcopy do? What are the arguments and outputs?
3. What does avr-ar do? What is the difference between linking a file.o at compile time vs linking a core library?
4. What are the various fuse settings - what do we want for our applications and why?


## Notes:

* You can use ```make --makefile MakefileX``` to specify which Makefile to use from these solutions
eg. ```make --makefile Makefile3 setfuse flash read clean``` to show solution to 3.
