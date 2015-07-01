#include <avr/io.h>

void bootOLED(uint8_t *ptr){
    for (uint8_t i = 0; i < 10; i++){
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
    bootOLED((uint8_t[]){0xAE,0xD5,0x80,0x8D,0x14,0x20,0x00,0x81,0xFF,0xAF});
    PORTB |=  (1 << PORTB2);

    PORTC |=  (1 << PORTC2); // OLED 'Data'
    PORTB &= ~(1 << PORTB2);

    for(uint16_t frame = 0;;frame++)
        for (uint8_t col = 8; col > 0; col--)
            for (uint8_t x = 128; x > 0; x--)
                SPDR = (( (( ( x+(frame>>col) )&8 )>>3) + (col&1) )&1 )*255; // Multi speed lines
}

