// Done!

#include <avr/io.h>
#include <util/delay.h>

int main(void) {

    DDRB  |=  (1 << PORTB5);

    // DDRB  |=  (1 << PORTB7);
    // DDRD  |=  (1 << PORTD5);
    // PORTB |=  (1 << PORTB7);
    // PORTD |=  (1 << PORTD5);

    for (;;) {
        PORTB |=  (1 << PORTB5);
        // PORTB |=  (1 << PORTB7);
        // PORTD &=  ~(1 << PORTD5);
        _delay_ms(500);
        PORTB &= ~(1 << PORTB5);
        // PORTB &= ~(1 << PORTB7);
        // PORTD |=  (1 << PORTB5);
        _delay_ms(500);
    }

    return 0;
}
