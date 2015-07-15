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
uint8_t RUNNING_AUTOMATED_TEST = 0;
// --

int main(void) {

    initUSART0();
    initPWM(TOP);

    for (;;) {}
    return 0;       // Remember this must never be reached in baremetal without an OS
}

// -- App specific read / output messages & processes
void readFromRxBuffer(){

    if (RUNNING_AUTOMATED_TEST){
        switch (systemTicks){
            case  20: USART0SendString("THREE...!\r\n");        break;
            case  30: USART0SendString("TWO...!\r\n");          break;
            case  40: USART0SendString("ONE...!\r\n");          break;
            case  50: duty = 1450; displayDuty();               break;
            case  90: USART0SendString("TAKE PHOTO NOW!\r\n");  break;
            case 100: duty = 1700; displayDuty();               break;
            case 140: USART0SendString("TAKE PHOTO NOW!\r\n");  break;
            case 150: duty = 1950; displayDuty();               break;
            case 190: USART0SendString("TAKE PHOTO NOW!\r\n");  break;
            case 200: duty = 1700; displayDuty();               break;
            case 240: USART0SendString("TAKE PHOTO NOW!\r\n");  break;
            case 250: duty = 1450; displayDuty();               break;
            case 290: USART0SendString("TAKE PHOTO NOW!\r\n");  break;
            case 300: {
                duty = 1050;
                USART0SendString("*\r\n* TEST STILL RUNNING!!!! STAY CLEAR OF ROTOR *\r\n*\r\n");
                break;
            }
            case 340: USART0SendString("TAKE PHOTO NOW!\r\n");  break;
            case 350: duty = 1450; displayDuty();               break;
            case 390: USART0SendString("TAKE PHOTO NOW!\r\n");  break;
            case 400: duty = 1700; displayDuty();               break;
            case 440: USART0SendString("TAKE PHOTO NOW!\r\n");  break;
            case 450: duty = 1950; displayDuty();               break;
            case 490: USART0SendString("TAKE PHOTO NOW!\r\n");  break;
            case 500: duty = 1700; displayDuty();               break;
            case 540: USART0SendString("TAKE PHOTO NOW!\r\n");  break;
            case 550: duty = 1450; displayDuty();               break;
            case 590: USART0SendString("TAKE PHOTO NOW!\r\n");  break;
            case 600:
                duty =  900;
                USART0SendString("FINISHED & DISARMED\r\n");
                RUNNING_AUTOMATED_TEST = 0;
                break;
        }
    }
    else if (rcvdPtr == readPtr) return;
    else {
        uint8_t u8TempData = *readPtr++;
        if (readPtr > (ringBufferRecv + RING_BUFF_SIZE)) readPtr = ringBufferRecv;  // Wrap the readPtr round the ring

        switch (u8TempData) {
            case ' ':
                duty =  900;
                displayDuty();
                displayStep();
                USART0SendString("FINISHED & DISARMED\r\n");
                RUNNING_AUTOMATED_TEST = 0;
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
            case 'G':
                systemTicks = 0; // So full test fits within 16bit timer without rollover
                RUNNING_AUTOMATED_TEST = 1;
                duty = 1050; // Ensure Armed
                USART0SendString("\r\n1. ESC sticker?\r\n");
                USART0SendString("\r\n2. Prop sticker?\r\n");
                USART0SendString("\r\n3. Motor Temp in Shot?\r\n");
                USART0SendString("\r\n4. ESC Temp in Shot?\r\n");
                USART0SendString("\r\n5. All meters ON and ZEROed?\r\n");
                USART0SendString("\r\nSTAND BACK...!\r\n");
                break;
            default:
                USART0QueueByte(u8TempData);
        }
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
