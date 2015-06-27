/*
    What?   : A UART0 header using interupts to send recieve via ring buffer.
    Device  : ATmega328, using the hardware UART0.
    Usage   : #include <DanUSARTv2.h>, `make` (this will both build then flash). Use `screen /dev/tty.** 15200` to connect over FTDI.
    Memory  :
    Elegance:
    Issues  :
*/

#include <avr/interrupt.h>  // ISR(), sei, cli, <avr/io.h> for port defs etc
#include <stdio.h>          // sprintf() and standard types

// -- Choose my UART BAUD rate
#define USART_BAUDRATE 115200
// Calc UBBR = ((F_CPU / (USART_BAUDRATE * 16UL))) - 1
// However, needs to be an int, and don't want risk of truncation down so easier to set manually (for higher BAUDs)
#define UBRR_VALUE 8
#define RING_BUFF_SIZE 200
// --


// -- Function prototype declarations
void initUSART0();
void USART0SendString(char *ptr);
uint8_t USART0QueueByte(uint8_t u8Data);
void readFromRxBuffer();
// --

// -- Globals (on the stack)
uint8_t ringBufferSend[RING_BUFF_SIZE], *writePtr = ringBufferSend, *sendPtr = ringBufferSend;
uint8_t ringBufferRecv[RING_BUFF_SIZE], *readPtr  = ringBufferRecv, *rcvdPtr = ringBufferRecv;
// --

// int main(void) {

//     initUSART0();

//     for (;;) {}
//     return 0;       // Remember this must never be reached in baremetal without an OS
// }

// -- App specific read / output messages & processes
// void readFromRxBuffer(){

//     if (rcvdPtr == readPtr) return;

//     uint8_t u8TempData = *readPtr++;
//     if (readPtr > (ringBufferRecv + RING_BUFF_SIZE)) readPtr = ringBufferRecv;  // Wrap the readPtr round the ring

//     switch (u8TempData) {
//         case '\r':
//         case '\n':
//             USART0SendString("\r\n");
//             break;
//         default:
//             USART0QueueByte(u8TempData);
//     }
// }
// --

// -- Send Byte / String down the UART
void USART0SendString(char *ptr){
    cli();
    while (USART0QueueByte(*ptr++)); // We're okay : string from displayDuty/Step stack is copied by value to buffer
    sei();
}
uint8_t USART0QueueByte(uint8_t u8Data){
    *writePtr++ = u8Data;
    if (writePtr > (ringBufferSend + RING_BUFF_SIZE)) writePtr = ringBufferSend;  // Wrap the writePtr round the ring
    UCSR0B |= (1 << UDRIE0);                                            // Set the UDRIE as we have a queue to send
    return u8Data;                                                      // Useful for USART0SendString() to terminate strings
}
// --

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
    UCSR0B |=   (1 << RXCIE0) | (1 << TXCIE0) | (0 << UDRIE0);
    // --
    sei();
}
void initPWM(uint8_t TOP){ // Also used for the system timer!!
    // Enable PD5 & 6 for output
    DDRD |= (1 << PORTD5) | (1 << PORTD6);

    // Set Compare Output Mode A/B
    TCCR0A |= (0 << COM0A1) | (0 << COM0A0) | (1 << COM0B1) | (0 << COM0B0);

    // Set Wave Guide Mode
    TCCR0B |= (1 << WGM02); TCCR0A |= (1 << WGM01) | (1 << WGM00);

    // Set Clock Select
    TCCR0B |= (1 << CS02) | (0 << CS01) | (0 << CS00);

    // Enable/Disable interupts OCA/OCB/Overflow
    TIMSK0 |= (0 << OCIE0B) + (0 << OCIE0A) + (1 << TOIE0);

    // TOP set by OCRA
    OCR0A = TOP;

    // Clear the Timer and Go!
    TCNT0 = 0;
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
    if (sendPtr != writePtr) {
        UDR0 = *sendPtr++;                                                          // Send the next byte on our ring buffer
        if (sendPtr > (ringBufferSend + RING_BUFF_SIZE)) sendPtr = ringBufferSend;  // Wrap the sendPtr round the ring
    }
    else UCSR0B &= ~(1 << UDRIE0); // After sending the last byte (or if the UDRIE fires when nothing queud) clear the UDRIE from firing again (until we next write)
}
// --

// -- Fires once when UDR0 contains a byte from Rx
ISR(USART_RX_vect){
    // Nothing more than copy the UDR byte into our ringBuffer
    *rcvdPtr++ = UDR0;
    if (rcvdPtr > (ringBufferRecv + RING_BUFF_SIZE)) rcvdPtr = ringBufferRecv;  // Wrap the rcvdPtr round the ring
    // readFromRxBuffer();
}
// --

// -- Fires when Timer 0 reaches TOP (ie every 2.5ms)
ISR(TIMER0_OVF_vect){
    static uint8_t counter = 0;
    if (counter++ == 40) {      // Circa 10Hz
        readFromRxBuffer();
        counter = 0;
    }
}
// --
