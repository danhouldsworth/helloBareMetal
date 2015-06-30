#include <avr/io.h>
#define _NOP() __asm__ __volatile__("nop");

void bootOLED(uint8_t *ptr, uint16_t count){
    while (count--){
        SPDR = *ptr++;
        while(!(SPSR & (1<<SPIF)));
    }
}

int main(void){
    PORTC = DDRC = (1 << DDC2);                             // OLED D/C
    PORTB = DDRB = (1 << DDB3) | (1 << DDB5) | (1 << DDB2); // MOSI SCK SS
    SPCR = (1 << SPE) | (1 << MSTR);
    SPSR = (1 << SPI2X);

    PORTC &= ~(1 << PORTC2); // OLED 'Commands'
    PORTB &= ~(1 << PORTB2);
    bootOLED((uint8_t[]){0xAE,0xD5,0x80,0xA8,0x3F,0xD3,0x0,0x40,0x8D,0x14,0x20,0x00,0xA1,0xC8,0xDA,0x12,0x81,0xFF,0xD9,0x22,0xDB,0x30,0x2E,0xA4,0xA6,0xAF,0x21,0x0,127,0x22,0x0,0x7}, 32);
    PORTB |=  (1 << PORTB2);

    PORTC |=  (1 << PORTC2); // OLED 'Data'
    PORTB &= ~(1 << PORTB2);

    uint8_t offsetx = 0;
    uint8_t offsety = 0;
    uint8_t XSPEED =1;
    uint8_t YSPEED =16;
    uint8_t speedx = XSPEED;
    uint8_t speedy = YSPEED;
    for(;;) {
        if (!speedx-- && XSPEED){
            speedx = XSPEED;
            offsetx++;
            if (offsetx==16) {offsetx = 0; XSPEED++;}
            if (XSPEED>64) offsetx = 0;
        }
        if (!speedy-- && YSPEED){
            speedy = YSPEED;
            offsety++;
            if (offsety==16) {offsety = 0; YSPEED--;}
        }
        for (uint8_t col = 0; col < 8; col++) {
            for (uint8_t x = 0, value =0; x<128; x++) {
                if (offsety>=8){
                    if (((((x+offsetx)&8)>>3) + (col&1))&1) SPDR = 255 >> (16-offsety);
                    else                                    SPDR = 255 << (offsety-8);
                }else {
                    if (((((x+offsetx)&8)>>3) + (col&1))&1) SPDR = 255 << (offsety-0);
                    else                                    SPDR = 255 >> (8-offsety);
                }
                while(!(SPSR & (1<<SPIF)));
            }

        }
    }
}

