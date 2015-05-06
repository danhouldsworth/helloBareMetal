// Done.

#include <avr/io.h>
#include <util/delay.h>

int main(void) {

    DDRB  |=  (1 << PORTB7);
    PORTB |=  (1 << PORTB7);
    DDRD  |=  (1 << PORTD5);
    PORTD |=  (1 << PORTD5);

    double duty_ON = 1.0;
    double period = 20.0;
    double delta = 1.0;

    for (;;duty_ON += delta){
        for (int i = 0; i<2;i++) {
            PORTB |=  (1 << PORTB7);
            _delay_ms(duty_ON);
            PORTB &= ~(1 << PORTB7);
            _delay_ms(period - duty_ON);
        }
        if (duty_ON > (period-1.0) || duty_ON < 1.0) delta =- delta;
    }
    return 0;
}
