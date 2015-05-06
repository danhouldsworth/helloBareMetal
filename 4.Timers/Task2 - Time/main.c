// Using timer to flash beats on 2LEDs - PD5/PB7

#include <avr/interrupt.h>
#include <util/delay.h>

#define Prescaler   256                     // Don't forget to match the setting below for CS0
#define MAX         256
#define TicksPerOVF F_CPU/(Prescaler * MAX)

#define period_A   20
#define period_B   21


int duty_A, duty_B;

void setup(){
    // Enable PD5 & 6 for output
    DDRD |= (1 << PORTD5);
    DDRB |= (1 << PORTB7);

    // Set Compare Output Mode A/B
    TCCR0A |= (0 << COM0A1) | (0 << COM0A0) | (0 << COM0B1) | (0 << COM0B0);

    // Set Wave Guide Mode (Mode 0 = Normal )
    TCCR0B |= (0 << WGM02); TCCR0A |= (0 << WGM01) | (0 << WGM00);

    // Set Clock Select
    TCCR0B |= (1 << CS02) | (0 << CS01) | (0 << CS00);

    // Enable interupts OCA/OCB/Overflow
    TIMSK0 |= (0 << OCIE0B) + (0 << OCIE0A) + (1 << TOIE0);

    // TOP set by MAX in Mode3
    // Clear the Timer and Go!
    TCNT0 = 0;
    sei();
}

int main(void) {

    setup();

    for (;;) {}

    return 0;
}

ISR(TIMER0_OVF_vect){
    if (duty_A-- == 0) {
        duty_A = period_A;
        PORTD ^= (1<<PORTD5);
    }
    if (duty_B-- == 0) {
        duty_B = period_B;
        PORTB ^= (1<<PORTB7);
    }

}
