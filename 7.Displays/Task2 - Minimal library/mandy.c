#include <avr/interrupt.h>
#include <string.h>
#include <math.h>
#include "DanUSARTv2.h"

#define ON  1
#define OFF 0
#define maxDwell 100

typedef struct {
    float x;
    float y;
} complex32;

// -- Function declarations
float ratio(int a, int b);
uint8_t isMandy(complex32 c);
complex32 mapToArgand(int x, int y);
complex32 *complexAdd(complex32 *a, complex32 *b);
complex32 *complexSq(complex32 *z);
float complexModSq(complex32 *z);

void SSD1306_begin();
void SSD1306_display(void);
void SSD1306_drawPixel(uint8_t x, uint8_t y);
void writeSPIstream(uint8_t *, uint16_t);
void initHW_SPI(void);
void CSport(uint8_t state);
void DCport(uint8_t state);
void RSTport(uint8_t state);

// the memory buffer for the LCD
static uint8_t buffer[64 * 128 / 8];

int main(void){

        initUSART0();
        initPWM(80);
        initHW_SPI();
        SSD1306_begin();

        memset(buffer,0,sizeof(buffer));

        complex32 z0 = mapToArgand(0,0);

        for (uint8_t x = 0; x<128; x++){
            for (uint8_t y = 0; y<64; y++){
                if (isMandy(mapToArgand(x,y))) SSD1306_drawPixel(x,y);
                SSD1306_display();
            }
        }

        for(;;) {}
}

// -- App specific read / output messages & processes
void readFromRxBuffer(){}
// --

void CSport(uint8_t state){
	if (state == ON) PORTB |=  (1 << PORTB2);
	else PORTB &= ~(1 << PORTB2);
}
void DCport(uint8_t state){
	if (state == ON) PORTC |=  (1 << PORTC2);
	else PORTC &= ~(1 << PORTC2);
}
void RSTport(uint8_t state){
	if (state == ON) PORTC |=  (1 << PORTC3);
	else PORTC &= ~(1 << PORTC3);
}

void SSD1306_drawPixel(uint8_t x, uint8_t y) {
	if ((x < 0) || (x >= 128) || (y < 0) || (y >= 64)) return;
	else buffer[x+ (y/8)*128] |=  (1 << (y&7));
}

void SSD1306_begin(){
    RSTport(OFF);   waitTicks(1);   RSTport(ON);

    DCport(OFF);    CSport(OFF);
	writeSPIstream((uint8_t[]){0xAE,0xD5,0x80,0xA8,0x3F,0xD3,0x0,0x40,0x8D,0x14,0x20,0x00,0xA1,0xC8,0xDA,0x12,0x81,0xcF,0xD9,0xf1,0xDB,0x40,0x2E,0xA4,0xA6,0xAF}, 26);
    CSport(ON);
}

void SSD1306_display(void) {
    DCport(OFF); CSport(OFF);
    writeSPIstream((uint8_t[]){0x21,0,127,0x22,0,7}, 6);
	CSport(ON);

    DCport(ON);  CSport(OFF);
	writeSPIstream(buffer, 128*64/8);
	CSport(ON);
}

void initHW_SPI(){
	//	          DC 		    RST
	DDRC  |= (1 << DDC2) | 	(1 << DDC3);
    PORTC |= (1 << DDC2) |  (1 << DDC3);

	//  	    MOSI  	    SCK 		     SS
	DDRB |=	(1 << DDB3) | (1 << DDB5) | (1 << DDB2);
	PORTB = 0;

	SPCR = (0 << SPIE) | (1 << SPE) | (1 << MSTR) | (0 << SPR1) | (0 << SPR0) | (0 << CPOL) | (0 << CPHA);
}

uint8_t hwSPI(uint8_t cData){
	SPDR = cData;
	while(!(SPSR & (1<<SPIF)));
	return  SPDR; // Clear the SPIF by reading SPDR
}

void writeSPIstream(uint8_t *ptr, uint16_t count){
    cli();
    while (count--) hwSPI(*ptr++);
    sei();
}
uint8_t isMandy(complex32 c) {
    int dwell = 0;
    for (complex32 z = c; complexModSq(&z) < 4; z = *complexAdd(complexSq(&z), &c)) if (dwell++ >= maxDwell) return 1;
    return 0;
}

/*
    Take (int) screen pixel positions, and map to (float) complex z + iy in the mandy argand plane.
*/
complex32 mapToArgand(int x, int y) {
    // This could still be called ~millions, so could make more efficient. Although -O3 should handle this.
    complex32 z0;
    complex32 mandyMIN = {-2,-0.75};
    complex32 mandyMAX = {1,0.75};
    z0.x = mandyMIN.x + ratio(x, 128) * (mandyMAX.x - mandyMIN.x);
    z0.y = mandyMIN.y + ratio(y, 64) * (mandyMAX.y - mandyMIN.y);
    return z0;
}


/* These complex32 operations will be used BILLIONS of times, so be careful about memory usage/leakage */
/*
    a += b
*/
complex32 *complexAdd(complex32 *a, complex32 *b){
    a->x = a->x + b->x;
    a->y = a->y + b->y;
    return a;
}

/*
    z *= z
*/
complex32 *complexSq(complex32 *z){
    float temp = z->x * z->x - z->y * z->y;
    z->y = 2 * z->y * z->x;
    z->x = temp;
    return z;
}

/*
    return |z|^2
*/
float complexModSq(complex32 *z){
    return z->x * z->x + z->y * z->y;
}

/*
    A reminder to self to cast, as I tend to forget a/b === 0 for ints where a<b
*/
float ratio(int a, int b) {
    return (float)a / (float)b;
}


