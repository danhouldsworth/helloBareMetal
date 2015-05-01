.include "../Reference/Libs/m328Pdef.inc"

.equ    counter    = 0xffff             ; Makes 65536x(2+4+2)(x2 on/off) = ~1million cycles. So blink @ 8Hz

.cseg
.org 0x0000
jmp    setup

.org PCI1addr
jmp    buttonChange

setup:
        cbi     DDRC,PINC1                ; PORTC as input
        sbi     PORTC,PINC1               ; PINC as high (We're using PC1)

        sbi     DDRB, PORTB7
        sbi     DDRD, PORTD5

        ldi     r16, (1 << PCIE1)
        ldi     ZH, HIGH(PCICR)
        ldi     ZL, LOW(PCICR)
        st      Z, r16

        ldi     r16, (1 << PCINT9)
        ldi     ZH, HIGH(PCMSK1)
        ldi     ZL, LOW(PCMSK1)
        st      Z, r16

        sei

loop:   rjmp    loop

lamp:
        sbi     PORTB,PORTB7

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
        cbi     PORTB,PORTB7

        ret

buttonChange:
        sbis    PINC, PINC1 ; GND = pressed, so Bit clear is pressed, and call lamp. Skip if set aka off.
        rcall   lamp
        reti
