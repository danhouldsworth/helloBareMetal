.DEVICE ATmega328p                      ; Using LEDs on PB7 or PD5

.def    temp            = r16
.def    counter         = r17           ; Use .def to label a register
.def    dutyCycle       = r18
.def    direction       = r20

.equ    DDRB    = 0x04                  ; Use .equ to label a value
.equ    PORTB   = 0x05
.equ    PINB7   = 7
.equ    period  = 0xff
.equ    FullOFF = 0x00
.equ    UP      = 0x01
.equ    DOWN    = 0x00

.cseg
.org 0x0000
        jmp     setup                   ; rjmp for ATmega8 and ATtiny VS jmp for ATmega328 and ATmega32u4

.org 0x0100

setup:
        sbi     DDRB, PINB7

        ldi     dutyCycle, FullOFF      ; Full off
        ldi     direction, UP           ; going up

mainLoop:

        rcall   LEDon                   ; ON for dutycycle
        mov     counter, dutyCycle
        inc     counter                 ; Delay range from 1->256
waitON:
        rcall   nops8
        dec     counter
        brne    waitON

        rcall   LEDoff                  ; OFF for period - dutycycle
        ldi     counter, period
        sub     counter, dutyCycle
        inc     counter                 ; Delay range from 1->256
waitOFF:
        rcall   nops8
        dec     counter
        brne    waitOFF

        cpi     direction, UP
        breq    goingUp
goingDown:
        dec     dutyCycle
        cpi     dutyCycle, FullOFF
        brne    continuePulse
        ldi     direction, UP
        rjmp    mainLoop

goingUp:
        inc     dutyCycle
        cpi     dutyCycle, period
        brne    continuePulse
        ldi     direction, DOWN
        rjmp    mainLoop

continuePulse:
        rjmp    mainLoop

LEDon:
        sbi     PORTB,PINB7
        ret
LEDoff:
        cbi     PORTB,PINB7
        ret


; **** Delays ****
; "rcall 1nop"  [=== rcall(3), nop(1) ret(4)] will insert/delay 8 ticks before the next instruction (@8MHz ~ 1us)
; "rcall 2nops" [=== rcall(3), rcall(3), nop(1) ret(4) nop(1) ret(4)] will be 16 ticks (~2us)
; "rcall 4nops" [=== rcall(3) rcall(3) rcall(3) nop(1) ret(4) nop(1) ret(4) rcall(3) nop(1) ret(4) nop(1) ret(4) ] ~32tick / 4us
; So... rcall fullSec is ~1s @8MHz clock :-)
; ****************

fullSec:rcall halfSec
halfSec:rcall quart_S
quart_S:rcall tenth_S
tenth_S:rcall del64ms
del64ms:rcall del32ms
del32ms:rcall del16ms
del16ms:rcall del_8ms
del_8ms:rcall del_4ms
del_4ms:rcall del_2ms
del_2ms:rcall delayms
delayms:rcall nops512
nops512:rcall nops256
nops256:rcall nops128
nops128:rcall nops64
nops64: rcall nops32
nops32: rcall nops16
nops16: rcall nops8
nops8:  rcall nops4
nops4:  rcall nops2
nops2:  rcall nops1
nops1:  nop
        ret

