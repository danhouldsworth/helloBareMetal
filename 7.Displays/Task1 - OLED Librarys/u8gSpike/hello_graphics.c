#include "../u8glib/src/u8g.h"
#include <avr/interrupt.h>

u8g_t u8g;

void u8g_prepare(void) {
    u8g_SetFont(&u8g, u8g_font_6x10);
    u8g_SetFontRefHeightExtendedText(&u8g);
    u8g_SetDefaultForegroundColor(&u8g);
    u8g_SetFontPosTop(&u8g);
}

void u8g_box_frame(uint8_t a) {
    u8g_DrawStr(&u8g,   0, 0, "drawBox");
    u8g_DrawBox(&u8g,   5,      10, 20, 10);
    u8g_DrawBox(&u8g,   10+a,   15, 30, 7);
    u8g_DrawStr(&u8g,   0, 30, "drawFrame");
    u8g_DrawFrame(&u8g, 5,      10+30,  20, 10);
    u8g_DrawFrame(&u8g, 10+a,   15+30,  30, 7);
}

void u8g_string(uint8_t a) {
    u8g_DrawStr(&u8g, 30+a,31, " 0");
    u8g_DrawStr90(&u8g, 30,31+a, " 90");
    u8g_DrawStr180(&u8g, 30-a,31, " 180");
    u8g_DrawStr270(&u8g, 30,31-a, " 270");
}

void u8g_line(uint8_t a) {
    u8g_DrawStr(&u8g, 0, 0, "drawLine");
    u8g_DrawLine(&u8g, 7+a, 10, 40, 55);
    u8g_DrawLine(&u8g, 7+a*2, 10, 60, 55);
    u8g_DrawLine(&u8g, 7+a*3, 10, 80, 55);
    u8g_DrawLine(&u8g, 7+a*4, 10, 100, 55);
}

void u8g_ascii_1() {
    char s[] = " ";
    u8g_DrawStr(&u8g, 0, 0, "ASCII page 1");

    for(uint8_t y = 0; y < 6; y++ ) {
        for(uint8_t x = 0; x < 16; x++ ) {
            s[0] = y*16 + x + 32;
            u8g_DrawStr(&u8g, x*7, y*10+10, s);
        }
    }
}

void u8g_ascii_2() {
    char s[] = " ";
    u8g_DrawStr(&u8g, 0, 0, "ASCII page 2");

    for(uint8_t y = 0; y < 6; y++ ) {
        for(uint8_t x = 0; x < 16; x++ ) {
            s[0] = y*16 + x + 160;
            u8g_DrawStr(&u8g, x*7, y*10+10, s);
        }
    }
}

uint8_t draw_state = 0;

void draw(void) {
    u8g_prepare();
    switch(draw_state >> 3) {
        case 0: u8g_box_frame(draw_state&7);break;
        case 1: u8g_string(draw_state&7);   break;
        case 2: u8g_line(draw_state&7);     break;
        case 3: u8g_ascii_1();              break;
        case 4: u8g_ascii_2();              break;
    }
}

int main(void){
    u8g_InitHWSPI(&u8g, &u8g_dev_ssd1306_128x64_hw_spi, PN(2,1), PN(2,2), PN(2,3));
    for(;;){
        u8g_FirstPage(&u8g);
        do{
            draw();
        } while ( u8g_NextPage(&u8g) );

        if ( ++draw_state >= 5*8) draw_state = 0;
        u8g_Delay(150);
    }
}
