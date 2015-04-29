// We are going to toggle input on PCINT9 / PinC1

#include <avr/interrupt.h>
#include <util/delay.h>

void setup(){
    PCICR  = (1 << PCIE1);       // Enables PCINT8..14
    PCMSK1 = (1 << PCINT9);
    sei();
}

ISR(PCINT1_vect){
    PORTB |=  (1 << PORTB7);
    _delay_ms(500);
    PORTB &= ~(1 << PORTB7);
}

int main(void) {

    setup();

    DDRB  |=  (1 << PORTB7);
    DDRC  &=  ~(1 << PORTC1);       // Clear PinC1 for input (high imedance)
    PORTC |=   (1 << PORTC1);       // Set PinC1 high (enable pull up resister)

    for (;;) {}

    return 0;
}
