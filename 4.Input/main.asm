.include "../Reference/Libs/m328Pdef.inc"
; .include "../Reference/Libs/tn85def.inc"

.def    LEDstate   = r17                ; Use .def to label a register
.def    BUTTONstate= r18

.equ    counter    = 0xffff             ; Makes 65536x(2+4+2)(x2 on/off) = ~1million cycles. So blink @ 8Hz

        rjmp    setup
setup:
        ldi     r16, 0x00
        out     DDRC,r16                ; PORTC as input
        ldi     r16, 0xff
        out     DDRB,r16
        out     DDRD,r16
        ldi     r16, (1 << 1)
        out     PORTC,r16               ; PINC as high (We're using PC1)
        ldi     LEDstate, (1 << 5)      ; We're on PB7 and PD5 so using the same mask will alternate

loop:
        com     LEDstate
        out     PORTB,LEDstate
        out     PORTD,LEDstate

        ldi     ZH,HIGH(counter)
        ldi     ZL,LOW(counter)

inner:
        in      r16, PINC
        andi    r16, (1 << 1)
        cpi     r16, 0x00                ; GND = pressed
        breq    button
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

        rjmp    loop

button:
        ; out     PORTD,LEDstate
        rjmp inner
        ; ret
