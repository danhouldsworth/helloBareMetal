.include "m328Pdef.inc"

.def    zeroReg = r1
.def    test1    = r16
.def    test2    = r15
.def    temp    = r17
.def    temp2   = r18
.def    xPos    = r19
.def    colPos  = r20
.def    cData   = r21
.def    temp16L = r24
.def    temp16H = r25
.def    frame16L= r26
.def    frame16H= r27

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
        rcall animate
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

animate:
        clr frame16H
        clr frame16L          ; uint16 frame = 0
bitmap:
        ldi colPos, 0x8         ; uint8 colPos = 8
column:
        clr test1
        sbrc colPos, 0
        inc test1                ; B = (col)&1 ? true : false

        movw temp16L, frame16L  ; uint16 temp = frame

        mov temp2, colPos       ; uint8 temp2 = colPos
frmLSRcol:
        lsr temp16H
        ror temp16L
        dec temp2
        brne frmLSRcol          ; frame >> colPos
        ; temp16L:H === frame>>colPos

        ldi xPos, 128           ; uint8 xPos = 128
line:
        clr test2
        sbrc temp16L, 3
        inc test2               ; A = (x+frame>>colPos)&(1<<3) ? true : false

        clr cData
        sbrs test1, 0            ; is A ^ B ?
        com cData
        sbrs test2, 0
        com cData
        out SPDR, cData         ; Assume our calcs are sufficient delay at SPI2X
        nop
        nop
        nop
        nop

        adiw temp16L, 1         ; temp16L:H = xPos + frame >> colPos

        dec xPos
        brne line
        dec colPos
        brne column

        adiw frame16L, 1
        rcall ms_4
        ; rcall ms_16
        rjmp bitmap

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

