// We are going to toggle input on PCINT9 / PinC1

#include <avr/interrupt.h>
#include <util/delay.h>

void setup(){
    PCICR  = (1 << PCIE1);       // Enables PCINT8..14
    PCMSK1 = (1 << PCINT9);
    sei();
}

ISR(PCINT1_vect){
    if (!(PINC & (1 << PINC1))){
        PORTD |=   (1 << PORTD5);
    } else {
        PORTD &=  ~(1 << PORTD5);
    }
    cli();
}

int main(void) {

    setup();

    DDRB  |=   (1 << PORTB7);
    DDRD  |=   (1 << PORTD5);
    PORTB &=  ~(1 << PORTB7);
    PORTD &=  ~(1 << PORTD5);

    DDRC  &=  ~(1 << PORTC1);       // Clear PinC1 for input (high imedance)
    PORTC |=   (1 << PORTC1);       // Set PinC1 high (enable pull up resister)

    for (;;) {
        PORTB = ~PORTB;
        _delay_ms(100);
        sei();
    }

    return 0;
}
