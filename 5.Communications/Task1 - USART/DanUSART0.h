// "DanUSART0.h"


// -- Choose my BAUD rate
#define USART_BAUDRATE 115200
// Calc UBBR = ((F_CPU / (USART_BAUDRATE * 16UL))) - 1
// However, needs to be an int, and don't want risk of truncation down so easier to set manually (for higher BAUDs)
#define UBRR_VALUE 8
// --

uint8_t sendQueue[] = "01234567890123456789";
uint8_t *buffPtr = sendQueue;
uint8_t *sendPtr = sendQueue;

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
            USART0SendString("SpaceBar");
            break;
        case '\r':
        case '\n':
            USART0SendByte('\r');
            USART0SendByte('\n');
            break;
        case 'a'...'z':
            USART0SendByte(u8TempData - 32);
            break;
        case '0'...'9':
            for (int i = 0; i < (u8TempData - '0') ; i++) USART0SendByte('*');
            break;
        case '<':
            PORTB |= (1 << PORTB5);
            break;
        case '>':
            PORTB &= ~(1 << PORTB5);
            break;
        default:
            USART0SendByte(u8TempData);
    }
}
