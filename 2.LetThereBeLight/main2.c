// Done!

#include <avr/io.h>

int main(void) {

    DDRB  |=  (1 << PORTB4);
    DDRB  |=  (1 << PORTB3);
    PORTB |=  (1 << PORTB4);
    PORTB |=  (1 << PORTB3);
    // PORTB &= ~(1 << PORTB4);
    // PORTB &= ~(1 << PORTB3);

	for (;;) {
		// loop();
	}

	return 0;
}
