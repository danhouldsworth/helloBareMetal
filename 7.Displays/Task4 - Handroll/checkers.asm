.include "m328Pdef.inc"

.def    test    = r15
.def    temp    = r16
.def    xPos    = r17
.def    colPos  = r18
.def    cData   = r19
.def    frame   = r20

.cseg
.org 0x0000
jmp    setup

setup:
        sbi DDRC, DDC2                  ; OLED D/C
        sbi PORTC, PORTC2

        sbi DDRB, DDB2                  ; SS
        sbi PORTB, PORTB2
        sbi DDRB, DDB3                  ; MOSI
        sbi PORTB, PORTB3
        sbi DDRB, DDB5                  ; SCK
        sbi PORTB, PORTB5

        ldi temp, (1 << SPE)
        ori temp, (1 << MSTR)
        out SPCR, temp

        ldi temp, (1 << SPI2X)
        out SPSR, temp

        cbi PORTC, PORTC2               ; OLED 'Commands'
        cbi PORTB, PORTB2               ; CS low
        rcall bootOLED
        sbi PORTB, PORTB2               ; CS high

        sbi PORTC, PORTC2               ; OLED 'Data'
        cbi PORTB, PORTB2               ; CS low
        rcall bitmap
        sbi PORTB, PORTB2               ; CS high

loopForever:
        rjmp    loopForever

hwSPI:
        out SPDR, cData
waitSPIF:
        in temp, SPSR
        sbrs temp, SPIF
        brne waitSPIF
        ret

bootOLED:
        ldi cData, 0xAE ; Screen OFF
        rcall hwSPI
        ldi cData, 0xD5 ; Set Fosc = Max, Clock Div = 1
        rcall hwSPI
        ldi cData, 0x80
        rcall hwSPI
        ldi cData, 0x8D ; Enable Charge Pump
        rcall hwSPI
        ldi cData, 0x14
        rcall hwSPI
        ldi cData, 0x20 ; Set page address mode = horizontal
        rcall hwSPI
        ldi cData, 0x00
        rcall hwSPI
        ldi cData, 0x81 ; Set contrast = MAX
        rcall hwSPI
        ldi cData, 0xFF
        rcall hwSPI
        ldi cData, 0xAF ; Screen ON
        rcall hwSPI
        ret

bitmap:
        ldi colPos, 0x8
column:
        ldi xPos, 128
line:
        ldi temp, 0
        sbrc xPos, 3
        inc temp
        sbrc colPos, 0
        inc temp

        ldi cData, 0
        sbrs temp, 0
        ldi cData, 255
        rcall hwSPI

        dec xPos
        brne line
        dec colPos
        brne column

        ret

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

