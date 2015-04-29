.include "../Reference/Libs/m328Pdef.inc"
; .include "../Reference/Libs/tn85def.inc"

.def    LEDstate   = r17                ; Use .def to label a register
.def    BUTTONstate= r18

.equ    counter    = 0xffff             ; Makes 65536x(2+4+2)(x2 on/off) = ~1million cycles. So blink @ 8Hz

.cseg
.org 0x0000
        jmp    setup
.org PCI1addr
        jmp    buttonChange

setup:
        ldi     r16, 0x00
        out     DDRC,r16                ; PORTC as input
        ldi     r16, (1 << PORTC1)
        out     PORTC,r16               ; PINC as high (We're using PC1)

        ldi     r16, (1 << PORTB7)
        out     DDRB,r16
        ldi     r16, (1 << PORTD5)
        out     DDRD,r16

        ldi     r16, (1 << PCIE1)
        ldi      ZH, HIGH(PCICR)
        ldi      ZL, LOW(PCICR)
        st       Z, r16

        ldi     r16, (1 << PCINT9)
        ldi      ZH, HIGH(PCMSK1)
        ldi      ZL, LOW(PCMSK1)
        st       Z, r16

        sei

loop:   rjmp     loop

lamp:
        ldi     LEDstate, (1 << PORTB7)      ; We're on PB7 and PD5 so using the same mask will alternate
        out     PORTB,LEDstate

        ldi     ZH,HIGH(counter)
        ldi     ZL,LOW(counter)

inner:
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
        brne    inner
        ldi     LEDstate, 0
        out     PORTB,LEDstate

        reti

buttonChange:
        in      r16, PINC
        andi    r16, (1 << PINC1)
        cpi     r16, 0x00                ; GND = pressed
        breq    lamp
        reti
