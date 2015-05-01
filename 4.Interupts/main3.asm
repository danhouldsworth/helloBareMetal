.include "../Reference/Libs/m328Pdef.inc"

.def    LEDstate   = r17                ; Use .def to label a register

.equ    counter    = 0xffff             ; Makes 65536x(2+4+2)(x2 on/off) = ~1million cycles. So blink @ 8Hz

.cseg
.org 0x0000
        jmp    setup
.org PCI1addr
        jmp    buttonChangeISR

setup:
        cbi     DDRC,PINC1                ; PORTC as input
        sbi     PORTC,PINC1               ; PINC as high (We're using PC1)

        sbi     DDRB,PORTB7
        sbi     DDRD,PORTD5

        ldi     r16, (1 << PCIE1)
        ldi     ZH, HIGH(PCICR)
        ldi     ZL, LOW(PCICR)
        st      Z, r16

        ldi     r16, (1 << PCINT9)
        ldi     ZH, HIGH(PCMSK1)
        ldi     ZL, LOW(PCMSK1)
        st      Z, r16

        sei

lamp1toggle:
        com     LEDstate
        out     PORTB,LEDstate

        ldi     ZH,HIGH(counter)
        ldi     ZL,LOW(counter)

inner1:
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        sbiw    ZL,1
        brne    inner1
        sei
        rjmp    lamp1toggle

buttonChangeISR:
        sbi     PORTD, PORTD5
        sbic    PINC, PINC1             ; GND = pressed on my breadboard ie Bit clear when pressed
        cbi     PORTD, PORTD5
        cli
        reti
