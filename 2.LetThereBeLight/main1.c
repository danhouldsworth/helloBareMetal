// Done!

#include <avr/io.h>

int main(void) {

    DDRB  |=  (1 << PORTB4); // Set output mode on Pin 18 (PORTB4)
    PORTB |=  (1 << PORTB4); // Pin High (LED ON if wired to ground)
    // PORTB &= ~(1 << PORTB4); // Pin Low (LED ON if wired to Vcc)

	for (;;) {
		// loop();
	}

	return 0;
}
