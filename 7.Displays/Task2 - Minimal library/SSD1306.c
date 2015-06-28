#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "DanUSARTv2.h"

#define ON  1
#define OFF 0

#define BLACK   0
#define WHITE   1
#define INVERSE 2

#define SSD1306_LCDWIDTH                128
#define SSD1306_LCDHEIGHT               64

#define SSD1306_SETLOWCOLUMN            0x00
#define SSD1306_SETHIGHCOLUMN           0x10
#define SSD1306_EXTERNALVCC             0x1
#define SSD1306_SWITCHCAPVCC            0x2

#define SSD1306_MEMORYMODE              0x20
#define SSD1306_COLUMNADDR              0x21
#define SSD1306_PAGEADDR                0x22

#define SSD1306_DEACTIVATE_SCROLL       0x2E
#define SSD1306_ACTIVATE_SCROLL         0x2F
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3
#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL  0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL    0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL     0x2A

#define SSD1306_SETSTARTLINE            0x40

#define SSD1306_SETCONTRAST             0x81
#define SSD1306_CHARGEPUMP              0x8D

#define SSD1306_SEGREMAP                0xA0
#define SSD1306_DISPLAYALLON_RESUME     0xA4
#define SSD1306_DISPLAYALLON            0xA5
#define SSD1306_NORMALDISPLAY           0xA6
#define SSD1306_INVERTDISPLAY           0xA7
#define SSD1306_SETMULTIPLEX            0xA8
#define SSD1306_DISPLAYOFF              0xAE
#define SSD1306_DISPLAYON               0xAF

#define SSD1306_COMSCANINC              0xC0
#define SSD1306_COMSCANDEC              0xC8

#define SSD1306_SETDISPLAYOFFSET        0xD3
#define SSD1306_SETDISPLAYCLOCKDIV      0xD5
#define SSD1306_SETPRECHARGE            0xD9
#define SSD1306_SETCOMPINS              0xDA
#define SSD1306_SETVCOMDETECT           0xDB


void SSD1306_begin();
void SSD1306_command(uint8_t c);
void SSD1306_display(void);
void SSD1306_drawPixel(uint8_t x, uint8_t y, uint8_t color);
void writeSPI(uint8_t c);
void initHW_SPI(void);
void CSport(uint8_t state);
void DCport(uint8_t state);
void RSTport(uint8_t state);

// the memory buffer for the LCD
static uint8_t buffer[SSD1306_LCDHEIGHT * SSD1306_LCDWIDTH / 8];

#define PARTICLES 50

float x[PARTICLES];
float y[PARTICLES];
float vx[PARTICLES];
float vy[PARTICLES];
float ax = 0.0;
float ay = 0.5;


int main(void){

        for (int i =0; i<PARTICLES;i++){
                x[i] = 64; y[i] = 32;
                vx[i] = sin((float)i * 2* M_PI/(float)PARTICLES);
                vy[i] = cos((float)i * 2* M_PI/(float)PARTICLES);
        }

        initUSART0();
        initPWM(255);
        initHW_SPI();
        SSD1306_begin();
        memset(buffer,0,sizeof(buffer));

        uint8_t p = 0;
        for(;;) {

                for (p = 0; p<PARTICLES; p++){
                        x[p] += vx[p];
                        y[p] += vy[p];
                        vx[p] += ax;
                        vy[p] += ay;
                        if (x[p]>128) {vx[p] *= -0.9; x[p] = 128;}
                        if (y[p]>64) {vy[p] *= -0.9; y[p] = 64;}
                        if (x[p]<1) {vx[p] *= -0.9; x[p] = 1;}
                        if (y[p]<1) {vy[p] *= -0.9; y[p] = 1;}

                        for (uint8_t i =0; i<1; i++){
                                for (uint8_t j=0; j<1; j++){
                                SSD1306_drawPixel((uint8_t)x[p]+i-1, (uint8_t)y[p]+j-1, WHITE);
                                }
                        }

                }
                SSD1306_display();
                memset(buffer,0,sizeof(buffer));
        }
}

// -- App specific read / output messages & processes
void readFromRxBuffer(){

        // char maxStr[10];

        if (rcvdPtr == readPtr) return;

        uint8_t u8TempData = *readPtr++;
        if (readPtr > (ringBufferRecv + RING_BUFF_SIZE)) readPtr = ringBufferRecv;  // Wrap the readPtr round the ring

        switch (u8TempData) {
        case '\r':
        case '\n':
                // sprintf(maxStr, "x = %d\r\n", x); USART0SendString(maxStr);
                // sprintf(maxStr, "y = %d\r\n", y); USART0SendString(maxStr);
                // sprintf(maxStr, "vx = %d\r\n", vx); USART0SendString(maxStr);
                // sprintf(maxStr, "vy = %d\r\n", vy); USART0SendString(maxStr);
        	USART0SendString("\r\n");
        	break;
        }
}
// --

void CSport(uint8_t state){
	if (state == ON) {
		PORTC |=  (1 << PORTC1);
		// USART0SendString("CS1\r\n");
	}
	else if (state == OFF) {
		PORTC &= ~(1 << PORTC1);
		// USART0SendString("CS0\r\n");
	}
	else USART0SendString("CS BUG!\r\n");
}
void DCport(uint8_t state){
	if (state == ON){
		PORTC |=  (1 << PORTC2);
		// USART0SendString("D/C1\r\n");
	}
	else if (state == OFF){
		PORTC &= ~(1 << PORTC2);
		// USART0SendString("D/C0\r\n");
	}
	else USART0SendString("DC BUG!\r\n");
}
void RSTport(uint8_t state){
	if (state == ON) {
		PORTC |=  (1 << PORTC3);
		// USART0SendString("RST1\r\n");
	}
	else if (state == OFF) {
		PORTC &= ~(1 << PORTC3);
		// USART0SendString("RST0\r\n");
	}
	else USART0SendString("RESET BUG!\r\n");
}

void SSD1306_drawPixel(uint8_t x, uint8_t y, uint8_t color) {
	if ((x < 0) || (x >= 128) || (y < 0) || (y >= 64)) return;
	else switch (color){
		case WHITE:   buffer[x+ (y/8)*128] |=  (1 << (y&7)); break;
		case BLACK:   buffer[x+ (y/8)*128] &= ~(1 << (y&7)); break;
		case INVERSE: buffer[x+ (y/8)*128] ^=  (1 << (y&7)); break;
	}
}

void SSD1306_begin(){
        RSTport(ON);
        // RSTport(OFF);
        RSTport(ON);
	SSD1306_command(SSD1306_DISPLAYOFF);
	SSD1306_command(SSD1306_SETDISPLAYCLOCKDIV);   SSD1306_command(0x80);
	SSD1306_command(SSD1306_SETMULTIPLEX);         SSD1306_command(0x3F);
	SSD1306_command(SSD1306_SETDISPLAYOFFSET);     SSD1306_command(0x0);
	SSD1306_command(SSD1306_SETSTARTLINE | 0x0);
	SSD1306_command(SSD1306_CHARGEPUMP);           SSD1306_command(0x14); //10
	SSD1306_command(SSD1306_MEMORYMODE);           SSD1306_command(0x00);
	SSD1306_command(SSD1306_SEGREMAP | 0x1);
	SSD1306_command(SSD1306_COMSCANDEC);
	SSD1306_command(SSD1306_SETCOMPINS);           SSD1306_command(0x12);
	SSD1306_command(SSD1306_SETCONTRAST);          SSD1306_command(0xcF); //0xcf //9f
	SSD1306_command(SSD1306_SETPRECHARGE);         SSD1306_command(0xf1); //0xf1 //22
	SSD1306_command(SSD1306_SETVCOMDETECT);        SSD1306_command(0x40);
	SSD1306_command(SSD1306_DEACTIVATE_SCROLL);
	SSD1306_command(SSD1306_DISPLAYALLON_RESUME);
	SSD1306_command(SSD1306_NORMALDISPLAY);
	SSD1306_command(SSD1306_DISPLAYON);
}

void SSD1306_command(uint8_t c) {
	CSport(ON);
	DCport(OFF);
	CSport(OFF);
	writeSPI(c);
	CSport(ON);
	DCport(ON);
}

void SSD1306_display(void) {
	SSD1306_command(SSD1306_COLUMNADDR);
	SSD1306_command(0);   			// Column start address (0 = reset)
	SSD1306_command(SSD1306_LCDWIDTH-1); 	// Column end address (127 = reset)

	SSD1306_command(SSD1306_PAGEADDR);
	SSD1306_command(0); 			// Page start address (0 = reset)
	SSD1306_command(7); 			// Page end address

	CSport(ON);
	DCport(ON);
	CSport(OFF);
	for (uint16_t i=0; i<(SSD1306_LCDWIDTH*SSD1306_LCDHEIGHT/8); i++) writeSPI(buffer[i]);
	CSport(ON);
	DCport(ON);
}

void initHW_SPI(){
	//	CS 		DC 		RST
	DDRC |= (1 << DDC1) | 	(1 << DDC2) | 	(1 << DDC3); 		// GPIO pins to control OLED slave
        RSTport(ON);
	//  	MOSI  		SCK 		SS
	DDRB |=	(1 << DDB3) | 	(1 << DDB5) | 	(1 << DDB2);   		// NOTE!! SPI will hang if SS not set as output
	PORTB = 0;//(1 << PORTB3)| 	(1 << PORTB5);//| 	(1 << PORTB2); 	// But doesn't seem to care if set low or high

	/* Enable SPI, Master, set clock rate fck/16 */
        PRR &= ~(1<<PRSPI);  // clear the power down bit
	SPCR = (0 << SPIE) | (1 << SPE) | (1 << MSTR) | (0 << SPR1) | (0 << SPR0) | (0 << CPOL) | (0 << CPHA);
	CSport(ON);
	DCport(ON);
        RSTport(ON);
}
void swSPI(uint8_t cData){
    for(uint8_t bit = 0x80; bit; bit >>= 1) {
      PORTB &= ~(1<<PORTB5); 	// Clk off
      if(cData & bit) 	PORTB |=  (1<<PORTB3);
      else        	PORTB &= ~(1<<PORTB3);
      PORTB |= (1<<PORTB5); 	// Clk on
    }
}
uint8_t hwSPI(uint8_t cData){
	SPDR = cData;
	while(!(SPSR & (1<<SPIF)));
	return  SPDR; // Clear the SPIF by reading SPDR
}

void writeSPI(uint8_t cData){
        cli();
	// SPCR = 0;swSPI(cData);// Disable hwSPI use swSPI
	hwSPI(cData);
        sei();
}
