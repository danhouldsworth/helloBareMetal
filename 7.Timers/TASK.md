# Task

1. Generate a PWM at 400hz with a 2000us duty cycle. Test correct output with a scope (check not inverted - eg 2ms high 0.5ms low)
2. Output 4 concurrent PWMs at duty cycles 500us, 1000us, 1500us, 2000us
3. Adjust duty cycles to 1498us, 1499us, 1500us, 1501us and test that precision is measurable.
4. Repeat 1 but with a saw-tooth wave.
5. Repeat with a sinusoidal wave.
6. Using an interupt on a timer overflow, create a function that is non-blocking and returns time elapsed since power on.
7. Using this time function, output morse code over 2 LEDs simultaneously. 'Hello world' on one, and 'SOS' on the other.


## Bonus Learning

* What is the watchdog timer. Demonstrate it will reset the chip if we fail to 'pat the dog' in time.
