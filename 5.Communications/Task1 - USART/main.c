// Done.

#include <avr/io.h>
#include <util/delay.h>


#define USART_BAUDRATE 115200
#define UBRR_VALUE 8 // (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

void USART0SendByte(uint8_t u8Data);
uint8_t USART0ReceiveByte();

int main(void) {

    uint8_t u8TempData;

    UBRR0L= UBRR_VALUE;
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
    UCSR0B  = (1 << RXEN0) | (1 << TXEN0);

    DDRB  |=  (1 << PORTB5);

    while(1) {
        // Receive data
        u8TempData = USART0ReceiveByte();

        switch (u8TempData) {
            case ' ':
                USART0SendByte('S');
                USART0SendByte('p');
                USART0SendByte('a');
                USART0SendByte('c');
                USART0SendByte('e');
                break;
            case 'a'...'z':
                USART0SendByte(u8TempData - 32);
                break;
            case '0'...'9':
                for (int i = 0; i < (u8TempData - '0') ; i++){
                    USART0SendByte('N');
                    USART0SendByte('U');
                    USART0SendByte('M');
                    USART0SendByte(i + '0');
                    USART0SendByte('\n');
                    USART0SendByte('\r');
                }
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

    return 0;
}


void USART0SendByte(uint8_t u8Data){
    //wait while previous byte is completed
    while( !(UCSR0A & (1 << UDRE0) )){};
    // Transmit data
    UDR0 = u8Data;
}

uint8_t USART0ReceiveByte(){
    // Wait for byte to be received
    while( !(UCSR0A & (1 << RXC0) )){};
    // Return received data
    return UDR0;
}
