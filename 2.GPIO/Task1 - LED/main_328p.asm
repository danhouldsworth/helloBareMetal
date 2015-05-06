.DEVICE ATmega328p

        ; jmp     directPortAccess
        jmp      memoryAddressing

        ; *******************************************************************************************
        ; ** Direct method - fastest, and easy with data sheet

directPortAccess:
        ldi     r16, 1 << 7     ; Set DDRB bit 7 (Pin PB7 to output)
        out     0x04,r16

        ldi     r16, 1 << 7     ; Set PORTB bit 7 (Pin PB7 to HIGH 5v)
        out     0x05,r16

loop:   rjmp    loop


        ; *******************************************************************************************
        ; ** Alternative method, just to show how we can access the I/O registers given the knowledge
        ; ** that they are held in the Data section of SRAM, after the 32 General Purpose Registers
        ; ** they start at 0x0020 - This helps understand the application in C a bit more :-)
        ; ** See page 18 of Data Sheet (Section7.3 - SRAM Data Memory)

memoryAddressing:
        ldi     r16, 1 << 7
        ldi     r31, HIGH(0x004 + 0x0020)
        ldi     r30, LOW(0x004 + 0x0020)
        st      Z, r16

        ldi     r16, 1 << 7
        ldi     r31, HIGH(0x005 + 0x0020)
        ldi     r30, LOW(0x005 + 0x0020)
        st      Z, r16

        rjmp    loop
