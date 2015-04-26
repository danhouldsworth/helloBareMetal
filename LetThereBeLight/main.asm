.DEVICE ATmega328p

        rjmp      main
main:
        ldi      r16, 1 << 4
        out      0x04,r16

        ldi      r16, 1 << 4
        out      0x05,r16

        ldi      r16, 0x00
        out      0x05,r16

loop:   rjmp      loop
