# Task

PreRead : Read and list all the interupts available for our chip

1. Enable the interupt for (and disable all others) a pin input. Act on it to flash a LED for 1 second.
2. Use the interrupt to have a push button reliably toggle between 2 LEDs.
3. Show the multitasking cababilities by controlling LED1, while LED2 is steadily blinking
4. Repeat but using the Serial Rx UART interupt. (Parking downhill!)


## Bonus

* Repeat with no libraries, including the vector definitions and ISR declarations!
* And again in assembler :-)


## Learning questions :

* What is the overhead of using interupts? (Ie. pushing all the regs into stack then taking them off)
* How can we put the CPU to sleep, and use an interupt to wake it up. Why would we do this?
* What happens if we don't capture an interupt ISR