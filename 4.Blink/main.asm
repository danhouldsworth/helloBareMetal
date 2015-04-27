.DEVICE ATmega328p

.nolist
; .include "../Reference/Libs/m328Pdef.inc"
.list
        rjmp    main
main:
        ldi     r16, 1 << 4             ; Set GPIO PinB4 to OUT
        out     0x04,r16

        mov     r17, r16                ; r17 is our LED state (aka PinB4 = ON) [For interest, use the same value for DDR]

                                        ; Outer loop of 100, inner loop of 40000 with SBIW & BRNE taking 2clocks each
                                        ; Makes 100x40000x4 = 16million cycles. Our chip is 16MHZ === ~1second :-)
loop:
        com     r17                     ; 1's complement (aka toggle all PortB Pin's)
        out     0x05,r17                ; write to the PortB Pins

        ldi     r18,100                 ; r18 is our outer loop counter

outer:
        ldi     r31,HIGH(40000)
        ldi     r30,LOW(40000)

inner:
        sbiw    r30,1
        brne    inner

        dec     r18
        brne    outer

        rjmp    loop

