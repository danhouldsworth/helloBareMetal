.include "m328Pdef.inc"

.def    LEDstate   = r17                ; Use .def to label a register

        jmp    setup
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
        rcall   fullsecond

pressed:
        in      r16, PINC
        andi    r16, (1 << 1)
        cpi     r16, 0x00                ; GND = pressed
        breq    pressed
        rjmp    loop


; *************************************** Wait / Delays - Calbirated to 8MHz clock ***************************************
; "rcall us_1"
; [=== rcall(3), nop(1) ret(4)] will take 8 ticks before the next instruction (@8MHz ~ 1us)
; "rcall us_2"
; [=== rcall(3), rcall(3), nop(1) ret(4) nop(1) ret(4)] =16 ticks (~2us)
; "rcall us_4"
; [=== rcall(3) rcall(3) rcall(3) nop(1) ret(4) nop(1) ret(4) rcall(3) nop(1) ret(4) nop(1) ret(4) ] =32tick (~4us)
;
; So... rcall fullsecond is ~1s @8MHz clock :-)  [Simply change to 9 nops, so us_1 === 16 ticks for a 16MHz clock]
; ************************************************************************************************************************

fullsecond:
ms_1024:rcall ms_512
ms_512: rcall ms_256
halfsecond:
ms_256: rcall ms_128
ms_128: rcall ms_64
ms_64:  rcall ms_32
ms_32:  rcall ms_16
ms_16:  rcall ms_8
ms_8:   rcall ms_4
ms_4:   rcall ms_2
ms_2:   rcall ms_1
millisecond:
ms_1:   rcall us_512
us_512: rcall us_256
us_256: rcall us_128
us_128: rcall us_64
us_64:  rcall us_32
us_32:  rcall us_16
us_16:  rcall us_8
us_8:   rcall us_4
us_4:   rcall us_2
us_2:   rcall us_1
microsecond:
us_1:   nop
        ret

