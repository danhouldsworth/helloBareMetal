; .include "../Reference/Libs/m328Pdef.inc"
.include "../Reference/Libs/tn85def.inc"

.def    LEDstate   = r17                ; Use .def to label a register

.equ    counter    = 0x9999             ; Makes 65536x(2+4+2)(x2 on/off) = ~1million cycles. So blink @ 8Hz

        jmp    main
main:
        ldi     r16, 0xff
        out     DDRB,r16
        ldi     LEDstate, (1 << 3)

loop:
        com     LEDstate
        out     PORTB,LEDstate

        ldi     ZH,HIGH(counter)
        ldi     ZL,LOW(counter)

inner:
        sbiw    ZL,1
        brne    inner

        rjmp    loop
