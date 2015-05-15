# Task

1. Generate a PWM at 400hz with a 2000us duty cycle. Test correct output with a scope (check not inverted)
2. Using a timer overflow interrupt, output beat frequencies on 2 LEDs (eg 2Hz and 2.1Hz)
3. Use CTC mode to manipulate a PDM output such as a sinusoidal dutycyle
4. Setup a Watchdog timer, that will reset the chip if don't press a button every second.
5. Signalling I. Set up a SECOND AVR, which listens on PCINT to the output of Ex.1 and then pulses a LED at half frequency.
6. Signalling II. Repeat but listening on the ICP (Input Timer/Counter0) instead.
