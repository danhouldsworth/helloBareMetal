// Done!

#include <avr/io.h>
#include <util/delay.h>

int main(void) {

    DDRB  |=   (1 << PORTB7);
    DDRD  |=   (1 << PORTD5);
    DDRC  &=  ~(1 << PORTC1);       // Clear PinC1 for input
    PORTB |=   (1 << PORTB7);
    PORTD &=  ~(1 << PORTD5);
    PORTC |=   (1 << PORTC1);       // Set PinC1 high
    // PORTC &=  ~(1 << PORTC1);       // Set PinC1 low

    for (;;) {
        PORTB = ~PORTB;
        PORTD = ~PORTD;
        _delay_ms(100);

        while (PINC & (1 << PINC1)) {}

    }

    return 0;
}
