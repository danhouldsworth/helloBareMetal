#include "../u8glib/src/u8g.h"
#include <avr/interrupt.h>

u8g_t u8g;

void draw(){
    u8g_SetFont(&u8g, u8g_font_gdr25r);
    u8g_DrawStr(&u8g, 0, 30, "D");
    u8g_DrawStr(&u8g, 53,30,"Yay!");

    u8g_SetFont(&u8g, u8g_font_gdr30n);
    u8g_DrawStr90(&u8g, 23,10, "8");

    u8g_DrawHLine(&u8g,  2, 35, 47);
    u8g_DrawVLine(&u8g, 45, 32, 12);

    u8g_SetFont(&u8g, u8g_font_4x6);
    u8g_DrawStr(&u8g,  1,54,"Bullet time drones");
    u8g_DrawStr(&u8g, 32,64,"by 2020 industries");
}

int main(void){
    u8g_InitHWSPI(&u8g, &u8g_dev_ssd1306_128x64_hw_spi, PN(2,1), PN(2,2), PN(2,3));

    u8g_FirstPage(&u8g);
    do{draw();} while ( u8g_NextPage(&u8g) );

    for(;;){}
}
