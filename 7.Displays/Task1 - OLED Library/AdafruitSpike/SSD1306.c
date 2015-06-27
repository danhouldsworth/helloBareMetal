#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include "SSD1306.h"
#include "DanUSARTv2.h"

#define ON  1
#define OFF 0

void SSD1306_begin();
void SSD1306_command(uint8_t c);
void SSD1306_data(uint8_t c);
void SSD1306_display(void);
void SSD1306_drawPixel(int16_t x, int16_t y, uint16_t color);
void writeSPI(uint8_t c);
void initHW_SPI(void);
void CSport(uint8_t state);
void DCport(uint8_t state);
void RSTport(uint8_t state);
void displaySPItotal(void);

// the memory buffer for the LCD
static uint8_t buffer[SSD1306_LCDHEIGHT * SSD1306_LCDWIDTH / 8];
uint16_t hwSPIbyteCounter = 0;
uint16_t swSPIbyteCounter = 0;
uint8_t reset_toggle = ON;
uint8_t shade  = 0;

int main(void){

	initUSART0();
	initPWM(255);

	for(;;){
	}
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
        	USART0SendString("Confirmed USART init()\r\n");
        	break;
        case '2':
	        USART0SendString("initHW_SPI()...\r\n");
        	initHW_SPI();
        	USART0SendString("done! \r\n");
    		displaySPItotal();
        	break;
        case '3':
            memset(buffer,shade++,sizeof(buffer));
            break;
        case 'r':
        	RSTport(reset_toggle);
        	if (reset_toggle == 1) {
        		reset_toggle = 0;
			USART0SendString("RESET HIGH\r\n");
        	}
        	else if (reset_toggle == 0) {
        		reset_toggle = 1;
			USART0SendString("RESET LOW\r\n");
        	}
        	break;
        case '4':
        	USART0SendString("SSD1306_begin() -->>[");
        	SSD1306_begin();
        	USART0SendString("\b]<<--\r\n");
        	displaySPItotal();
        	break;
        case '5':
        	USART0SendString("SSD1306_display() -->>[");
        	SSD1306_display();
        	USART0SendString("\b]<<--\r\n");
        	displaySPItotal();
        	break;
        case 't':
            sprintf(maxStr, "DDRB = %X\r\n", DDRB); USART0SendString(maxStr);
            sprintf(maxStr, "DDRC = %X\r\n", DDRC); USART0SendString(maxStr);
            sprintf(maxStr, "PORTB = %X\r\n", PORTB); USART0SendString(maxStr);
            sprintf(maxStr, "PORTC = %X\r\n", PORTC); USART0SendString(maxStr);
            sprintf(maxStr, "SPCR = %X\r\n", SPCR); USART0SendString(maxStr);
            sprintf(maxStr, "SPSR = %X\r\n", SPSR); USART0SendString(maxStr);
            break;
        case 'l':
        	// for (uint16_t i=0; i<SSD1306_LCDWIDTH; i+=8) writeSPI(buffer[i+row]);
        	// USART0SendString("\r\n");
        	// row++;
        	break;
        case 'd':
        	SSD1306_data(255);
        	displaySPItotal();
        	break;
        case 'c':
        	SSD1306_command(255);
        	displaySPItotal();
        	break;
        // default:
        	// USART0QueueByte(u8TempData);
    }
}
// --

void displaySPItotal(){
    char maxStr[100];
    sprintf(maxStr, "Cumulative bytes sent on hwSPI = %d\r\n", hwSPIbyteCounter);
    USART0SendString(maxStr);       // Careful : we're sending a pointer to the stack....
    sprintf(maxStr, "Cumulative bytes sent on swSPI = %d\r\n", swSPIbyteCounter);
    USART0SendString(maxStr);       // Careful : we're sending a pointer to the stack....
}

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

void SSD1306_drawPixel(int16_t x, int16_t y, uint16_t color) {
	if ((x < 0) || (x >= SSD1306_LCDWIDTH) || (y < 0) || (y >= SSD1306_LCDHEIGHT)) return;
	else switch (color){
		case WHITE:   buffer[x+ (y/8)*SSD1306_LCDWIDTH] |=  (1 << (y&7)); break;
		case BLACK:   buffer[x+ (y/8)*SSD1306_LCDWIDTH] &= ~(1 << (y&7)); break;
		case INVERSE: buffer[x+ (y/8)*SSD1306_LCDWIDTH] ^=  (1 << (y&7)); break;
	}
}

void SSD1306_begin(){
	SSD1306_command(SSD1306_DISPLAYOFF);
	SSD1306_command(SSD1306_SETDISPLAYCLOCKDIV);	SSD1306_command(0x80);
	SSD1306_command(SSD1306_SETMULTIPLEX);		SSD1306_command(0x3F);
	SSD1306_command(SSD1306_SETDISPLAYOFFSET);	SSD1306_command(0x0);
	SSD1306_command(SSD1306_SETSTARTLINE | 0x0);
	SSD1306_command(SSD1306_CHARGEPUMP);		SSD1306_command(0x14); //10
	SSD1306_command(SSD1306_MEMORYMODE);		SSD1306_command(0x00);
	SSD1306_command(SSD1306_SEGREMAP | 0x1);
	SSD1306_command(SSD1306_COMSCANDEC);
	SSD1306_command(SSD1306_SETCOMPINS);		SSD1306_command(0x12);
	SSD1306_command(SSD1306_SETCONTRAST); 		SSD1306_command(0xcF); //0xcf //9f
	SSD1306_command(SSD1306_SETPRECHARGE);		SSD1306_command(0xf1); //0xf1 //22

	SSD1306_command(SSD1306_SETVCOMDETECT);		SSD1306_command(0x40);
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

void SSD1306_data(uint8_t c) {
	CSport(ON);
	DCport(ON);
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
	PORTC = 1 << PORTC3; // Try and leave reset to manual
	// PRR &= ~(1<<PRSPI);  // clear the power down bit
	/* Set MOSI, SCK & SS as output and leave High*/
	//  	MOSI  		SCK 		SS
	DDRB |=	(1 << DDB3) | 	(1 << DDB5) | 	(1 << DDB2);   		// NOTE!! SPI will hang if SS not set as output
	PORTB = 0;//(1 << PORTB3)| 	(1 << PORTB5);//| 	(1 << PORTB2); 	// But doesn't seem to care if set low or high

	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (0 << SPIE) | (1 << SPE) | (1 << MSTR) | (0 << SPR1) | (0 << SPR0) | (0 << CPOL) | (0 << CPHA);
	// Disable hwSPI use swSPI
	// SPCR = 0;
	CSport(ON);
	DCport(ON);
}
void swSPI(uint8_t cData){
    for(uint8_t bit = 0x80; bit; bit >>= 1) {
      PORTB &= ~(1<<PORTB5); 	// Clk off
      if(cData & bit) 	PORTB |=  (1<<PORTB3);
      else        	PORTB &= ~(1<<PORTB3);
      PORTB |= (1<<PORTB5); 	// Clk on
    }
    swSPIbyteCounter++;
}
void uartSPI(uint8_t cData){
    for(uint8_t bit = 0x80; bit; bit >>= 1) {
      if(cData & bit) 	USART0QueueByte('.');
      else        	USART0QueueByte(' ');
    }
}
uint8_t hwSPI(uint8_t cData){
	/* Start transmission */
	cli();
	SPDR = cData;
	while(!(SPSR & (1<<SPIF)));
	sei();
	hwSPIbyteCounter++;
	// return 0;
	return  SPDR; // Clear the SPIF by reading SPDR
}

void writeSPI(uint8_t cData){
	// swSPI(cData);
	hwSPI(cData);
	// uartSPI(cData);
}
// ISR(SPI_STC_vect){}
