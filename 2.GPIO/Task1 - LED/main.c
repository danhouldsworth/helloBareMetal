// Done.

#include <avr/io.h>

int main(void) {

    DDRB  |=  (1 << PORTB5); // Set output mode on PORTB4
    PORTB |=  (1 << PORTB5); // Pin High (LED ON if wired to ground)
    // PORTB &= ~(1 << PORTB5); // Pin Low (LED ON if wired to Vcc)

	for (;;) {
		// loop();
	}

	return 0;
}
