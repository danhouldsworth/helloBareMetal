.DEVICE ATmega328p

.def    LEDstate   = r17                ; Use .def to label a register
.def    counter    = r18
.def    ZH         = r31
.def    ZL         = r30

.equ    DDR        = 0x04               ; Use .equ to label a value
.equ    PORTB      = 0x05
.equ    outer1byte = 25                 ; Outer loop of 25, inner loop of 40000 with SBIW & BRNE taking 2clocks each
.equ    inner2byte = 40000              ; Makes 25x40000x4(x2 on/off) = 8million cycles. Our chip is 8MHz === ~1second :-)

.cseg
.org 0x0000

        jmp    main
main:
        ldi     r16, 1 << 7             ; Set GPIO PinB7 to OUT
        out     DDR,r16

        mov     LEDstate, r16           ; r17 is our LED state (aka PinB4 = ON) [For interest, use the same value for DDR]

loop:
        com     LEDstate                ; 1's complement (aka toggle all PortB Pin's)
        out     PORTB,LEDstate          ; write to the PortB Pins

        ldi     counter,outer1byte      ; r18 is our outer loop counter

outer:
        ldi     ZH,HIGH(inner2byte)
        ldi     ZL,LOW(inner2byte)

inner:
        sbiw    ZL,1                    ; SuBtract Immediate from Word (ie 2 byte reg)
        brne    inner

        dec     counter
        brne    outer

        rjmp    loop
