#include <avr/interrupt.h>
#include <string.h>
#include <math.h>
#include "DanUSARTv2.h"

#define ON  1
#define OFF 0

void SSD1306_begin();
void SSD1306_display(void);
void SSD1306_drawPixel(uint8_t x, uint8_t y, uint8_t color);
void writeSPIstream(uint8_t *, uint16_t);
void initHW_SPI(void);
void CSport(uint8_t state);
void DCport(uint8_t state);
void RSTport(uint8_t state);

// the memory buffer for the LCD
static uint8_t buffer[64 * 128 / 8];

#define PARTICLES 15

float x[PARTICLES];
float y[PARTICLES];
float vx[PARTICLES];
float vy[PARTICLES];
float ax = 0.000;
float ay = 0.5;

int main(void){

        for (int i =0; i<PARTICLES;i++){
                x[i] = 64 + 3*sin((float)i * 2* M_PI/(float)PARTICLES);
                y[i] = 32 + 3*cos((float)i * 2* M_PI/(float)PARTICLES);
                vx[i] = 0;//0.1*sin((float)i * 2* M_PI/(float)PARTICLES);
                vy[i] = 0;//0.1*cos((float)i * 2* M_PI/(float)PARTICLES) - 2.5;
        }

        initUSART0();
        initPWM(80);
        initHW_SPI();
        SSD1306_begin();

        for(uint8_t p = 0;;) {

                for (p = 0; p<PARTICLES; p++){
                        x[p] += vx[p];
                        y[p] += vy[p];
                        vx[p] += ax;
                        vy[p] += ay;

                        for (uint8_t q = 0; q<PARTICLES; q++){
                            if (p == q) continue;
                            int8_t dx = x[q] - x[p];
                            int8_t dy = y[q] - y[p];
                            float sep = dx*dx+dy*dy;
                            if (dx) vx[q] += dx / sep;
                            if (dy) vy[q] += dy / sep;
                        }

                        if (x[p]>125){vx[p] *= -0.8; x[p] = 125;}
                        if (y[p]>61) {vy[p] *= -0.8; y[p] =  61;}
                        if (x[p]<1)  {vx[p] *= -0.8; x[p] =   1;}
                        if (y[p]<1)  {vy[p] *= -0.8; y[p] =   1;}

                        SSD1306_drawPixel((uint8_t)x[p]+0, (uint8_t)y[p]-1, 1);
                        SSD1306_drawPixel((uint8_t)x[p]+1, (uint8_t)y[p]-1, 1);
                        SSD1306_drawPixel((uint8_t)x[p]-1, (uint8_t)y[p]-0, 1);
                        SSD1306_drawPixel((uint8_t)x[p]+0, (uint8_t)y[p]-0, 1);
                        SSD1306_drawPixel((uint8_t)x[p]+1, (uint8_t)y[p]-0, 1);
                        SSD1306_drawPixel((uint8_t)x[p]+2, (uint8_t)y[p]-0, 1);
                        SSD1306_drawPixel((uint8_t)x[p]-1, (uint8_t)y[p]+1, 1);
                        SSD1306_drawPixel((uint8_t)x[p]+0, (uint8_t)y[p]+1, 1);
                        SSD1306_drawPixel((uint8_t)x[p]+1, (uint8_t)y[p]+1, 1);
                        SSD1306_drawPixel((uint8_t)x[p]+2, (uint8_t)y[p]+1, 1);
                        SSD1306_drawPixel((uint8_t)x[p]+0, (uint8_t)y[p]+2, 1);
                        SSD1306_drawPixel((uint8_t)x[p]+1, (uint8_t)y[p]+2, 1);

                }
                SSD1306_display();
                memset(buffer,0,sizeof(buffer));
        }
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

void SSD1306_drawPixel(uint8_t x, uint8_t y, uint8_t color) {
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

