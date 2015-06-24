#include "u8g.h"
#include <avr/interrupt.h>

u8g_t u8g;

int main(void){
    /* select minimal prescaler (max system speed) */
    CLKPR = 0x80;
    // CLKPR = 0x00;
    u8g_InitHWSPI(&u8g, &u8g_dev_ssd1306_128x64_hw_spi, PN(2,1), PN(2,2), PN(2,3));

    unsigned char x = 0;
    unsigned char y = 64;

    for(;;){
        u8g_FirstPage(&u8g);
        do{
            u8g_SetFont(&u8g, u8g_font_6x10);
            u8g_DrawStr(&u8g, x, y, "Hello Dan?");
        } while ( u8g_NextPage(&u8g) );
        y--;x++;
        u8g_Delay(100);
    }
}
