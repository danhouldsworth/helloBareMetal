/*
    What?   : A simple  based, ESC signal generator that takes instructions on UART.
    Device  : ATmega328, using the 8-bit Timer/Counter0 for PWM, and hardware UART0.
    Usage   : `make` (this will both build then flash). Use `screen /dev/tty.** 15200` to connect over FTDI.
    Memory  : All stack. See notes, as fairly clumsy use of pointers to stack, and non-ring buffer
    Elegance: Simple to read, understand and modify. Not suitable for production though.
    Issues  : I'm using an external crystal 16MHz, but the AfroESC with internal clock, have significant drift
            : so despite simpleSimonK set to 1100-1900 they seem to drift to 1070-1870
*/

#include <avr/interrupt.h>
#include <stdio.h>

// -- Choose my UART BAUD rate
#define USART_BAUDRATE 115200
// Calc UBBR = ((F_CPU / (USART_BAUDRATE * 16UL))) - 1
// However, needs to be an int, and don't want risk of truncation down so easier to set manually (for higher BAUDs)
#define UBRR_VALUE 8
// --

// -- Choose PWM settings for 400hz using 16Mhz / 8-bit counter
#define Prescaler   256                     // Don't forget to match the setting below for CS0
#define TargetHz    400                     // 400Hz === 2500us period
#define TOP         (F_CPU/Prescaler)/TargetHz
// --

// -- Function prototype declarations
void initUSART0();
void initPWM();
void USART0SendString(char *ptr);
uint8_t USART0SendByte(uint8_t u8Data);
// --

// -- Globals (on the stack)
uint16_t duty = 1000;                                                   // 1000us ON / 1500us OFF
uint8_t sendQueue[1000], *buffPtr = sendQueue, *sendPtr = sendQueue;    // 1000char max string
uint16_t stepSize = 10, MAX_PWM = 2000, MIN_PWM = 1000;
// --

int main(void) {

    initUSART0();
    initPWM();

    for (;;) {}
    return 0;       // Remember this must never be reached in baremetal without an OS
}

// -- App specific output messages
void displayDuty(){
    char maxStr[100];
    sprintf(maxStr, "Duty = %d or %d%% of throttle\r\n", duty, (uint16_t)((duty - MIN_PWM) / (MAX_PWM/100 - MIN_PWM/100)));
    USART0SendString(maxStr);       // Careful : we're sending a pointer to the stack....
}
void displayStep(){
    char maxStr[100];
    sprintf(maxStr, "StepSize = %d or %d%% of full throttle\r\n", stepSize, (uint16_t)(stepSize / (MAX_PWM/100 - MIN_PWM/100)));
    USART0SendString(maxStr);       // Careful : we're sending a pointer to the stack....
}
// --

// -- Send Byte / String down the UART
void USART0SendString(char *ptr){
    while (USART0SendByte(*ptr++)); // We're okay : string from displayDuty/Step stack is copied by value to buffer
}
uint8_t USART0SendByte(uint8_t u8Data){
    *(buffPtr++) = u8Data;
    UCSR0B |= (1 << UDRIE0);                    // Set the UDRIE as we have a queue to send
    return u8Data;                              // Useful for USART0SendString() to terminate strings
}
// --

// -- Initialisation
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
    OCR0B = (uint8_t)(TOP * duty/2500);

    // Clear the Timer and Go!
    TCNT0 = 0;
    sei();
}

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
}
// --

/*
ISR declarations : Note! If you dont catch an enabled interrupt it will run in to other code. Likely another ISR!!
*/
// -- USART_TX_vect will only fire once, when Tx has been fully sent.
ISR(USART_TX_vect){}
// --

// -- USART_UDRE_vect fires whenever the UDR is empty (and will continue to do so unless we clear UDRIE)
ISR(USART_UDRE_vect){
    if (sendPtr < buffPtr) UDR0 = *sendPtr++;   // Send the next byte in our send buffer

    if (sendPtr == buffPtr){                    // After sending the last byte (or if the UDRIE fires when nothing queud)
        sendPtr = buffPtr = sendQueue;          // reset the read/write heads to start of buffer
        UCSR0B &= ~(1 << UDRIE0);               // and clear the UDRIE from firing again (until we next write)
    }
    /* Issues with above approach :
    + Simple to understand, implement and debug
    + Easy to just increase buffer size if we need bigger / slower messages
    - If sendPtr is always lagging buffPtr (ie when sending a continuous stream),
    its easy to see that we could quickly consume the buffer and stack overflow.

    Alternative approach would be to use a ring buffer :
    + Same space utilisation
    + Same buffer for unsent messages
    + Tolerates continuous stream as long as sendPtr doesn't lag more than bufferSize behind.
    */
}
// --

// -- Fires once when UDR0 contains a byte from Rx
ISR(USART_RX_vect){
    /* This is a bit heavy really for an ISR, we would want to be sure that the functions :
    displayStep(), displayDuty() & USART0SendByte()
    did not take too long. Fine for this simple app but bad practice really.
    */
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
            duty = 950;     // Try a deliberately low or...
            displayDuty();
            break;
        case 'h':
        case 'H':
            duty = 2050;    // ...high setting to test the ESC
            displayDuty();
            break;
        default:
            USART0SendByte(u8TempData);
    }

    OCR0B = (uint8_t)(TOP * duty/2500);
}
// --
