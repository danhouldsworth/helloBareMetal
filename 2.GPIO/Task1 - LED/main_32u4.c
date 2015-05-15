// Done.

#include <avr/io.h>

int main(void) {

    DDRC  |=  (1 << 7);
    PORTC |=  (1 << 7);

	for (;;) {
		// loop();
	}

	return 0;
}
