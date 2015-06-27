/*
    What?   : A simple AVR based, ESC signal generator that takes instructions on UART.
    Device  : ATmega328, using the 8-bit Timer/Counter0 for PWM, and hardware UART0.
    Usage   : `make` (this will both build then flash). Use `screen /dev/tty.** 15200` to connect over FTDI.
    Memory  : All stack. Use of ring buffers minimises SRAM use, application strings are in PROGMEM
    Elegance: Simple to read, understand and modify. ALL ISR's return promptly.
    Issues  : I'm using an external crystal 16MHz, but the AfroESC with internal clock, have significant drift
            : so despite simpleSimonK set to 1100-1900 they seem to drift to 1070-1870
*/

#include <avr/interrupt.h>  // ISR(), sei, cli, <avr/io.h> for port defs etc
#include <stdio.h>          // sprintf() and standard types
#include "DanUSARTv2.h"

// -- Choose PWM settings for 400hz using 16Mhz / 8-bit counter
#define Prescaler   256                     // Don't forget to match the setting below for CS0
#define TargetHz    400                     // 400Hz === 2500us period
#define TOP         (F_CPU/Prescaler)/TargetHz
// --

// -- Function prototype declarations
void initPWM(uint8_t);
void displayDuty();
void displayStep();
// --

// -- Globals (on the stack)
uint16_t duty = 1000;   // 1000us ON / 1500us OFF
uint16_t stepSize = 10, MAX_PWM = 2000, MIN_PWM = 1000;
// --

int main(void) {

    initUSART0();
    initPWM(TOP);

    for (;;) {}
    return 0;       // Remember this must never be reached in baremetal without an OS
}

// -- App specific read / output messages & processes
void readFromRxBuffer(){

    if (rcvdPtr == readPtr) return;

    uint8_t u8TempData = *readPtr++;
    if (readPtr > (ringBufferRecv + RING_BUFF_SIZE)) readPtr = ringBufferRecv;  // Wrap the readPtr round the ring

    switch (u8TempData) {
        case ' ':
            displayDuty();
            displayStep();
            break;
        case '\r':
        case '\n':
            USART0SendString("\r\n");
            break;
        case '1': stepSize = 1;         displayStep();break;
        case '2': stepSize = 10;        displayStep();break;
        case '3': stepSize = 100;       displayStep();break;
        case '4': stepSize = 500;       displayStep();break;
        case '5': stepSize = 1000;      displayStep();break;
        case '-':
        case '_':
        case ',':
        case '<':
            duty -= stepSize;
            if (duty < MIN_PWM) duty = MIN_PWM;
            displayDuty();
            break;
        case '=':
        case '+':
        case '.':
        case '>':
            duty += stepSize;
            if (duty > MAX_PWM) duty = MAX_PWM;
            displayDuty();
            break;
        case 'l':
        case 'L':
            duty = 950;     // Try a deliberately low or...
            displayDuty();
            break;
        case 'h':
        case 'H':
            duty = 2050;    // ...high setting to test the ESC
            displayDuty();
            break;
        default:
            USART0QueueByte(u8TempData);
    }

    OCR0B = (uint8_t)(TOP * duty/2500);
}
void displayDuty(){
    char maxStr[100];
    sprintf(maxStr, "Duty = %d or %d%% of throttle\r\n", duty, (uint16_t)((duty - MIN_PWM) / (MAX_PWM/100 - MIN_PWM/100)));
    USART0SendString(maxStr);       // Careful : we're sending a pointer to the stack....
}
void displayStep(){
    char maxStr[100];
    sprintf(maxStr, "StepSize = %d or %d%% of full throttle\r\n", stepSize, (uint16_t)(stepSize / (MAX_PWM/100 - MIN_PWM/100)));
    USART0SendString(maxStr);       // Careful : we're sending a pointer to the stack....
}
// --

// -- Initialisation
// void initPWM(uint8_t top){
//     // Enable PD5 & 6 for output
//     DDRD |= (1 << PORTD5) | (1 << PORTD6);

//     // Set Compare Output Mode A/B
//     TCCR0A |= (0 << COM0A1) | (0 << COM0A0) | (1 << COM0B1) | (0 << COM0B0);

//     // Set Wave Guide Mode
//     TCCR0B |= (1 << WGM02); TCCR0A |= (1 << WGM01) | (1 << WGM00);

//     // Set Clock Select
//     TCCR0B |= (1 << CS02) | (0 << CS01) | (0 << CS00);

//     // Enable/Disable interupts OCA/OCB/Overflow
//     TIMSK0 |= (0 << OCIE0B) + (0 << OCIE0A) + (1 << TOIE0);

//     // top set by OCRA
//     OCR0A = top;
//     OCR0B = (uint8_t)(top * duty/2500);

//     // Clear the Timer and Go!
//     TCNT0 = 0;
//     sei();
// }
