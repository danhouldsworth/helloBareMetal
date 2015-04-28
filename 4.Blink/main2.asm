.DEVICE ATmega328p

.def    LEDstate   = r17                ; Use .def to label a register
.def    counter    = r18
.def    ZH         = r31
.def    ZL         = r30

.equ    DDR        = 0x04               ; Use .equ to label a value
.equ    PORTB      = 0x05

.equ    counter    = 0xffff             ; Makes 65536x(2+4+2)(x2 on/off) = ~1million cycles. So blink @ 8Hz

        rjmp    main
main:
        ldi     r16, 1 << 4
        out     DDR,r16
        mov     LEDstate, r16

loop:
        com     LEDstate
        out     PORTB,LEDstate

        ldi     ZH,HIGH(counter)
        ldi     ZL,LOW(counter)

inner:
        sbiw    ZL,1
        nop
        nop
        nop
        nop
        brne    inner

        rjmp    loop
