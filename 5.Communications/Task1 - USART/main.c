#include <avr/io.h>
#include <util/delay.h>

#define USART_BAUDRATE 115200
#define UBRR_VALUE 8 // (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

void USART0SendString(char * ptr);
uint8_t USART0SendByte(uint8_t u8Data);
uint8_t USART0ReceiveByte();

int main(void) {

    uint8_t u8TempData;

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
    // -- but disable the interrupts for now
    UCSR0B &= ~((1 << RXCIE0)| (1 << TXCIE0) | (1 << UDRIE0));
    // --

    // -- Set LED pin to output
    DDRB  |=  (1 << PORTB5);
    // --

    while(1) {
        u8TempData = USART0ReceiveByte();

        switch (u8TempData) {
            case ' ':
                USART0SendString("SpaceBar");
                break;
            case 'a'...'z':
                USART0SendByte(u8TempData - 32);
                break;
            case '0'...'9':
                for (int i = 0; i <= (u8TempData - '0') ; i++) USART0SendByte('*');
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


void USART0SendString(char *ptr){
    while (USART0SendByte(*ptr++)){}
}

uint8_t USART0SendByte(uint8_t u8Data){
    // Wait until the USART Data Register Empty flag is set
    while( !(UCSR0A & (1 << UDRE0) )){}
    // Write byte to UART Data Register Buffer
    return (UDR0 = u8Data);
}

uint8_t USART0ReceiveByte(){
    // Wait until the Rx Complete flag is set
    while( !(UCSR0A & (1 << RXC0) )){};
    // Read byte from UART Data Register Buffer
    return UDR0;
}
