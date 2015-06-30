.include "m328Pdef.inc"

.def    zeroReg = r1
.def    test    = r16
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
        ldi xPos, 128           ; uint8 xPos = 128
line:
        movw temp16L, frame16L  ; uint16 temp = frame

        mov temp2, colPos       ; uint8 temp2 = colPos
frmLSRcol:
        lsr temp16L             ; Lose the LSB of 16bit temp
        bst temp16H, 0         ; ensure bit8 makes it across from tempH to tempL
        bld temp16L, 7
        lsr temp16H             ; Finish shift 16bit temp to the right

        dec temp2
        brne frmLSRcol          ; frame >> colPos

        add temp16L, xPos       ; xPos + frame >> colPos
        adc temp16H, zeroReg    ; (Careful to add the carry bit...)

        clr test

        sbrc temp16L, 3
        inc test                ; A = (x+frame)&(1<<3) ? true : false

        sbrc colPos, 0
        inc test                ; B = (col)&1 ? true : false

        clr cData
        sbrs test, 0            ; is A ^ B ?
        ldi cData, 255
        out SPDR, cData         ; Assume our calcs are sufficient delay at SPI2X

        dec xPos
        brne line
        dec colPos
        brne column

        adiw frame16L, 1
        rjmp bitmap

        ret
