; Pulse a pin with 2000us HIGH, 500us low via the interupt
        ; Clock Want circa 80tick per counter => 20000tick per overflow => 2.5ms period
        ; Set at Clock/64 which will be bit fast => 2.0ms
        ; OR could do at Clock/256 with TOP at 80  => 2.5ms

.include "../Reference/Libs/m328Pdef.inc"

.def    temp            = r16

.equ    dutycycle       = 204                 ; 0xff * 2000us/2500us
.equ    ON              = 1
.equ    OFF             = 0

; ***************************** Output Compare - Channel A *****************************
;
.equ    COMA = 0   ;          Normal Pin. OCA disconnected
; .equ    COMA = 1   ;          Toggle OC0A on Compare Match
; .equ    COMA = 2   ;            Clear OC0A on Compare Match. "Non-inverted PWM"
; .equ    COMA = 3   ;          Set OC0A on Compare Match "Inverted PWM"
;
; **************************************************************************************

; ***************************** Output Compare - Channel B *****************************
;
; .equ    COMB = 0   ; Normal Pin. OCA disconnected
; .equ    COMB = 1   ; Toggle OC0A on Compare Match
.equ    COMB = 2   ; Clear OC0A on Compare Match. "Non-inverted PWM"
; .equ    COMB = 3   ; Set OC0A on Compare Match "Inverted PWM"
;
; **************************************************************************************

; ***************************** Clock Select *******************************************
;
; .equ    CS = 0   ; Stopped
; .equ    CS = 1   ; x1 (No prescaler)
.equ    CS = 2   ; /8
; .equ    CS = 3   ; /64
; .equ    CS = 4   ; /256
; .equ    CS = 5   ; /1024
; .equ    CS = 6   ; External Clock T0 - falling
; .equ    CS = 7   ; External Clock T0 - rising
;
; **************************************************************************************

; *********************************** Wave Guide Mode  *********************************
;
;                       Description              TOP=    Update OCR      TOV
;
; .equ WGM = 0 ;        Normal                   0xFF    TOP             MAX
; .equ WGM = 1 ;        PWM, Phase Correct       0xFF    TOP             BOTTOM
; .equ WGM = 2 ;        CTC                      OCRA    Immediate       MAX
.equ WGM = 3 ;          Fast PWM                 0xFF    BOTTOM          MAX
; .equ WGM = 4 ;        Reserved
; .equ WGM = 5 ;        PWM, Phase Correct       OCRA    TOP             BOTTOM
; .equ WGM = 6 ;        Reserved
; .equ WGM = 7 ;        Fast PWM                 OCRA    BOTTOM          TOP
;
; **************************************************************************************

; *********************************** Interrupt Capture ********************************
.equ ISR_output_A = OFF
.equ ISR_output_B = OFF
.equ ISR_OverFlow = OFF
; **************************************************************************************


.cseg
.org 0x0000
        jmp     setup                   ; rjmp for ATmega8 and ATtiny VS jmp for ATmega328 and ATmega32u4
.org OC0Aaddr
        jmp     TIMER0_COMPA_ISR
.org OC0Baddr
        jmp     TIMER0_COMPB_ISR
.org OVF0addr
        jmp     TIMER0_OVF_ISR

.org 0x0100

setup:
        sbi     DDRD, PORTD6            ; Enable OC0A aka PD6 for output
        sbi     DDRD, PORTD5            ; Enable OC0B aka PD5 for output

        ; Reset the counter
        ldi     temp, 0
        out     TCNT0, temp

        ; Set OCR for dutycycle
        ldi     temp, dutycycle
        out     OCR0A, temp             ; TOP=OCR0A in WGMode 13
        out     OCR0B, temp

        ; Set the Timer/Counter Control Register for WaveGuideMode, Clock Select & Compare settings
        ldi     temp, ( (COMA & 2)>>1 ) << COM0A1
        ori     temp, ( (COMA & 1)    ) << COM0A0
        ori     temp, ( (COMB & 2)>>1 ) << COM0B1
        ori     temp, ( (COMB & 1)    ) << COM0B0
        ori     temp, ( (WGM  & 2)>>1 ) << WGM01
        ori     temp, ( (WGM  & 1)    ) << WGM00
        out     TCCR0A, temp
        ldi     temp, ( (WGM  & 4)>>2 ) << WGM02
        ori     temp, ( (CS   & 4)>>2 ) << CS02
        ori     temp, ( (CS   & 2)>>1 ) << CS01
        ori     temp, ( (CS   & 1)    ) << CS00
        out     TCCR0B, temp

        ; Enable interupts for OCRA / OCRB and Timer overflow
        ldi     temp, (ISR_output_B << OCIE0B) + (ISR_output_A << OCIE0A) + (ISR_OverFlow << TOIE0)
        ldi     ZH, HIGH(TIMSK0)
        ldi     ZL, LOW(TIMSK0)
        st      Z, temp

        ; Finally enable global interrupts
        sei

        ; ******** END OF SETUP **********

loopForever:
        rcall   del32ms
        in      temp, OCR0B
        inc     temp
        out     OCR0B, temp
        rjmp    loopForever


TIMER0_COMPA_ISR:
        reti

TIMER0_COMPB_ISR:
        reti

TIMER0_OVF_ISR:
        reti

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

