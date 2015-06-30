#include "u8g.h"
#include <avr/interrupt.h>
#include "DanUSARTv2.h"

void readFromRxBuffer();
void basicWrite(char *msg);

u8g_t u8g;

int main(void){

    initUSART0();
    initPWM(255);

    // basicWrite("Hello!");

    for(;;){}
}

void basicWrite(char *msg){
    u8g_FirstPage(&u8g);
    do{
        u8g_SetFont(&u8g, u8g_font_gdr30);
        u8g_DrawStr(&u8g, 0, 64, msg);
    } while ( u8g_NextPage(&u8g) );

}

// -- App specific read / output messages & processes
void readFromRxBuffer(){

    if (rcvdPtr == readPtr) return;

    uint8_t u8TempData = *readPtr++;
    if (readPtr > (ringBufferRecv + RING_BUFF_SIZE)) readPtr = ringBufferRecv;  // Wrap the readPtr round the ring

    char maxStr[100];
    switch (u8TempData) {

        case '\r':
        case '\n':
            USART0SendString("\r\n");
            break;
        case '1':
            USART0SendString("u8g_InitHWSPI() -->>[");
            u8g_InitHWSPI(&u8g, &u8g_dev_ssd1306_128x64_hw_spi, PN(2,1), PN(2,2), PN(2,3));
            USART0SendString("\b]<<--\r\n");
            break;
        case '2':
            USART0SendString("'Hello'...");
            basicWrite("Hello");
            USART0SendString("..done!\r\n");
            break;
        case '3':
            USART0SendString("', world!'...");
            basicWrite(", world!");
            USART0SendString("..done!\r\n");
            break;
        case 't':
            sprintf(maxStr, "DDRB = %X\r\n", DDRB); USART0SendString(maxStr);
            sprintf(maxStr, "DDRC = %X\r\n", DDRC); USART0SendString(maxStr);
            sprintf(maxStr, "PORTB = %X\r\n", PORTB); USART0SendString(maxStr);
            sprintf(maxStr, "PORTC = %X\r\n", PORTC); USART0SendString(maxStr);
            sprintf(maxStr, "SPCR = %X\r\n", SPCR); USART0SendString(maxStr);
            sprintf(maxStr, "SPSR = %X\r\n", SPSR); USART0SendString(maxStr);
            break;

        // default:
            // USART0QueueByte(u8TempData);
    }
}
// --

