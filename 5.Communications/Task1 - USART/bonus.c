#include <avr/interrupt.h>
// #include <util/delay.h>
#include <stdio.h>


#define Prescaler   256                     // Don't forget to match the setting below for CS0
#define TargetHz    400                     // 400Hz === 2500us period
#define TOP         (F_CPU/Prescaler)/TargetHz
#define dutycycle   TOP * 1000/2500         // 2000us ON / 500us OFF
uint16_t duty = 1000;

void initPWM(){
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

// -- Choose my BAUD rate
#define USART_BAUDRATE 115200
// Calc UBBR = ((F_CPU / (USART_BAUDRATE * 16UL))) - 1
// However, needs to be an int, and don't want risk of truncation down so easier to set manually (for higher BAUDs)
#define UBRR_VALUE 8
// --

uint8_t sendQueue[1000], *buffPtr = sendQueue, *sendPtr = sendQueue;
uint16_t stepSize = 10, MAX_PWM = 2000, MIN_PWM = 1000;

void initUSART0(){
    // -- Set BAUD (Let GCC handle the low byte / high byte assigment of 16bit reg)
    UBRR0 = UBRR_VALUE;
    // --

    // -- Set the mode (00 = Async)
    UCSR0C &= ~((1 << UMSEL01)| (1 << UMSEL00));
    // -- Set the Parity (00 = OFF)
    UCSR0C &= ~((1 << UPM01)  | (1 << UPM00));
    // -- Set the stop bit (0 = 1stop)
    UCSR0C &= ~( 1 << USBS0);
    // -- Set the Character Size bits (011 = 8bits)
    UCSR0C |=   (1 << UCSZ01) | (1 << UCSZ00);
    // --

    // -- Enable the PINs for Rx and Tx
    UCSR0B |=   (1 << RXEN0) | (1 << TXEN0);
    // -- and the interrupts
    UCSR0B |=   (1 << RXCIE0) | (0 << TXCIE0) | (0 << UDRIE0);
    // --
    sei();

    // Write a test byte
    // USART0SendByte(32);
    // UDR0 = 32;
}
uint8_t USART0SendByte(uint8_t u8Data){
    *(buffPtr++) = u8Data;
    UCSR0B |= (1 << UDRIE0);
    return u8Data;
}
void USART0SendString(char *ptr){
    while (USART0SendByte(*ptr++));
}
void displayDuty(){
    char maxStr[100];
    sprintf(maxStr, "Duty = %d or %d%% of throttle\r\n", duty, (uint16_t)((duty - MIN_PWM) / (MAX_PWM/100 - MIN_PWM/100)));
    USART0SendString(maxStr);
}
void displayStep(){
    char maxStr[100];
    sprintf(maxStr, "StepSize = %d or %d%% of full throttle\r\n", stepSize, (uint16_t)(stepSize / (MAX_PWM/100 - MIN_PWM/100)));
    USART0SendString(maxStr);
}


// -- Note! If you dont catch an enabled interrupt it will run in to other code. Likely another ISR!!
ISR(USART_TX_vect){
    // This will only fire once, when Tx has been fully sent.
}
ISR(USART_UDRE_vect){
    if (sendPtr < buffPtr) {
        UDR0 = *sendPtr++;
    }
    if (sendPtr == buffPtr){            // After sending the last byte (or if the UDRIE fires when nothing queud
        sendPtr = buffPtr = sendQueue;  // reset the read/write heads to start of buffer
        UCSR0B &= ~(1 << UDRIE0);       // and clear the UDRIE from firing again (until we next write)
    }
}
// --
ISR(USART_RX_vect){
    // return;
    uint8_t u8TempData = UDR0;

    switch (u8TempData) {
        case ' ':
            displayDuty();
            displayStep();
            break;
        case '\r':
        case '\n':
            USART0SendByte('\r');
            USART0SendByte('\n');
            break;
        case '1': stepSize = 1;         displayStep();break;
        case '2': stepSize = 10;        displayStep();break;
        case '3': stepSize = 100;       displayStep();break;
        case '4': stepSize = 500;       displayStep();break;
        case '5': stepSize = 1000;      displayStep();break;
        case '-':
        case '_':
        case ',':
        case '<':
            duty -= stepSize;
            if (duty < MIN_PWM) duty = MIN_PWM;
            displayDuty();
            break;
        case '=':
        case '+':
        case '.':
        case '>':
            duty += stepSize;
            if (duty > MAX_PWM) duty = MAX_PWM;
            displayDuty();
            break;
        case 'l':
        case 'L':
            duty = 950;
            displayDuty();
            break;
        case 'h':
        case 'H':
            duty = 2050;
            displayDuty();
            break;
        default:
            USART0SendByte(u8TempData);
    }

    OCR0B = (uint8_t)(TOP * duty/2500);
}


int main(void) {

    initUSART0();
    initPWM();

    for (;;) {}

    return 0;
}
