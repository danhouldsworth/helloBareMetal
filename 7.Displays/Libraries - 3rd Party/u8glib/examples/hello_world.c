#include "u8g.h"
#include <avr/interrupt.h>

u8g_t u8g;

int main(void){
    u8g_InitHWSPI(&u8g, &u8g_dev_ssd1306_128x64_hw_spi, PN(2,1), PN(2,2), PN(2,3));

    for(;;){
        u8g_FirstPage(&u8g);
        do{
            u8g_SetFont(&u8g, u8g_font_6x10);
            u8g_DrawStr(&u8g, 0, 64, "Hello Dan 12345?");
        } while ( u8g_NextPage(&u8g) );
        u8g_Delay(100);
    }
}
