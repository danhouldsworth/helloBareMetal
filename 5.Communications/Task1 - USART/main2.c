#include <avr/interrupt.h>
#include <util/delay.h>
#include "DanUSART0.h"

int main(void) {

    initUSART0();
    // -- Set LED pin to output
    DDRB  |=  (1 << PORTB5);
    // --

    for (;;) {
        PORTB |=  (1 << PORTB5);
        _delay_ms(5);
        PORTB &= ~(1 << PORTB5);
        _delay_ms(5);
    }

    return 0;
}
