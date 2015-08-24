// Timer not working?
// 5 ?
// 6 ?
// 3-->7
// 4-->8
// Progression jumps

#include <avr/interrupt.h>  // ISR(), sei, cli, <avr/io.h> for port defs etc

uint16_t channelVals[8] = {0,0,0,0,0,0,0,0};
uint8_t satFrame[16];
uint8_t *satByte = satFrame;

void initUSART0();
void computeRC();
void displayChannels();

void bootOLED(uint8_t *ptr){
    for (uint8_t i = 0; i < 10; i++){
        SPDR = *ptr++;
        while(!(SPSR & (1<<SPIF)));
    }
}

int main(void){
    PORTB = DDRB = (1 << DDB3) | (1 << DDB5) | (1 << DDB2) | (1 << DDB1); // MOSI SCK SS D/C
    SPCR = (1 << SPE) | (1 << MSTR);
    SPSR = (1 << SPI2X);

    PORTB &= ~(1 << PORTB1); // OLED 'Commands'
    PORTB &= ~(1 << PORTB2);
    bootOLED((uint8_t[]){0xAE,0xD5,0x80,0x8D,0x14,0x20,0x00,0x81,0xFF,0xAF});
    PORTB |=  (1 << PORTB2);

    initUSART0();

    for (;;) {
        // testDummyRC();
        computeRC();
        displayChannels();
    }

}

void initUSART0(){
    // -- Set BAUD (Let GCC handle the low byte / high byte assigment of 16bit reg)
    UBRR0 = 3; // Datasheet 7.3-8.0MHz for 115200 BAUD;
    // --

    // -- Set the mode (00 = Async)
    UCSR0C &= ~((1 << UMSEL01)| (1 << UMSEL00));
    // -- Set the Parity (00 = OFF)
    UCSR0C &= ~((1 << UPM01)  | (1 << UPM00));
    // -- Set the stop bit (0 = 1stop)
    UCSR0C &= ~( 1 << USBS0);
    // -- Set the Character Size bits (011 = 8bits)
    UCSR0C |=   (1 << UCSZ01) | (1 << UCSZ00);
    // --

    // -- Enable the PINs for Rx and the interrupt
    UCSR0B |=   (1 << RXEN0) | (1 << RXCIE0);
    // --
    sei();
}

// -- Fires once when UDR0 contains a byte from Rx
ISR(USART_RX_vect){
    *satByte++ = UDR0;
    if (satByte == satFrame + 16) { // complete frame
        satByte = satFrame;
    }

}
void testDummyRC(){
    channelVals[0]+=10;
    for (uint8_t channel = 0; channel < 7; channel++){
        if (channelVals[channel] >= 2047) {
            channelVals[channel] = 0;
            channelVals[channel + 1]+=10;
        }
    }
}
void computeRC(){
    // if (satFrame[0] != 0x03) return;
// // uint8_t channel =0;
    for (uint8_t frameByte = 3; frameByte < 16; frameByte +=2){
        uint8_t channel = 0b111 & (satFrame[frameByte - 1] >> 3);
        // if (channel == 0){
        channelVals[channel] = ((uint32_t)(satFrame[frameByte - 1] & 0b111 ) << 8) + satFrame[frameByte];
        // }
    }

    // for (uint8_t channel = 0; channel < 8; channel++){
        // channelVals[channel] = satFrame[channel];
    // }
}
void displayChannels(){


    PORTB |=  (1 << PORTB1); // OLED 'Data'
    PORTB &= ~(1 << PORTB2);
    for (uint8_t col = 0; col < 8; col++){
        for (uint8_t x = 0; x < 128; x++){
            if (x < (uint8_t)(channelVals[col] >>4)) {
                SPDR = 255;
            // } else if (x == (channelVals[col] >> 4)) {
                // SPDR = 255;// >> (7 - (channelVals[col] & 0b00001111) >> 2);
            } else {
                SPDR = 0;
            }
            while(!(SPSR & (1<<SPIF)));
        }
    }
    PORTB |=  (1 << PORTB2);
}
// --
