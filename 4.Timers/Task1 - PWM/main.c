#include <avr/interrupt.h>
#include <util/delay.h>

#define Prescaler   256                     // Don't forget to match the setting below for CS0
#define TargetHz    400                     // 400Hz === 2500us period
#define TOP         (F_CPU/Prescaler)/TargetHz
#define dutycycle   TOP * 2000/2500         // 2000us ON / 500us OFF

void setup(){
    // Enable PD5 & 6 for output
    DDRD |= (1 << PORTD5) | (1 << PORTD6);

    // Set Compare Output Mode A/B
    TCCR0A |= (0 << COM0A1) | (0 << COM0A0) | (1 << COM0B1) | (0 << COM0B0);

    // Set Wave Guide Mode
    TCCR0B |= (1 << WGM02); TCCR0A |= (1 << WGM01) | (1 << WGM00);

    // Set Clock Select
    TCCR0B |= (1 << CS02) | (0 << CS01) | (0 << CS00);

    // Enable interupts OCA/OCB/Overflow
    TIMSK0 |= (0 << OCIE0B) + (0 << OCIE0A) + (0 << TOIE0);

    // TOP set by OCRA
    OCR0A = TOP;
    OCR0B = dutycycle;

    // Clear the Timer and Go!
    TCNT0 = 0;
    sei();
}

int main(void) {

    setup();

    for (;;) {}

    return 0;
}
