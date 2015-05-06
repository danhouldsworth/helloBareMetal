.include "m328Pdef.inc"

.cseg
.org 0x0000
jmp    setup

.org PCI1addr
jmp    buttonChangeState

setup:
        cbi     DDRC,PINC1                      ; PORTC as input
        sbi     PORTC,PINC1                     ; PINC1 as high (internal pullup)

        sbi     DDRB, PORTB7                    ; PINB7 as output and ON
        sbi     DDRD, PORTD5

        ldi     r16, (1 << PCIE1)               ; PCICR = (1 << PCIE1) [enable PinChangeInterupt 1]
        ldi     ZH, HIGH(PCICR)
        ldi     ZL, LOW(PCICR)
        st      Z, r16

        ldi     r16, (1 << PCINT9)              ; PCMSK1 = (1 << PCINT9) [enable Pin 9 to fire PCINT]
        ldi     ZH, HIGH(PCMSK1)
        ldi     ZL, LOW(PCMSK1)
        st      Z, r16

        sei

background_forever_loop:
        rjmp    background_forever_loop

lamp:
        sbi     PORTB,PORTB7
        rcall   halfsecond
        cbi     PORTB,PORTB7
        ret

buttonChangeState:                              ; Note : Button wired to GRND
        sbis    PINC, PINC1                     ; Skip if Bit Set (ie not pressed)...
        rcall   lamp                            ; Only call lamp if buttonChangeState == Pressed
        rcall   ms_16                           ; Crude but effective debouncing on release :-)
        reti                                    ; return from interrupt (to background task)


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

