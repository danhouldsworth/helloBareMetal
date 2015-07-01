	.file	"fast850HzOLED.c"
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__SREG__ = 0x3f
__tmp_reg__ = 0
__zero_reg__ = 1
	.text
.global	bootOLED
	.type	bootOLED, @function
bootOLED:
	push r28
	push r29
	rcall .
	push __zero_reg__
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 3 */
/* stack size = 5 */
.L__stack_usage = 5
	std Y+3,r25
	std Y+2,r24
	std Y+1,__zero_reg__
	rjmp .L2
.L4:
	ldi r24,lo8(78)
	ldi r25,0
	ldd r18,Y+2
	ldd r19,Y+3
	movw r30,r18
	ld r18,Z
	movw r30,r24
	st Z,r18
	ldd r24,Y+2
	ldd r25,Y+3
	adiw r24,1
	std Y+3,r25
	std Y+2,r24
	nop
.L3:
	ldi r24,lo8(77)
	ldi r25,0
	movw r30,r24
	ld r24,Z
	tst r24
	brge .L3
	ldd r24,Y+1
	subi r24,lo8(-(1))
	std Y+1,r24
.L2:
	ldd r24,Y+1
	cpi r24,lo8(10)
	brlo .L4
/* epilogue start */
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop r29
	pop r28
	ret
	.size	bootOLED, .-bootOLED
	.section	.rodata
.LC0:
	.byte	-82
	.byte	-43
	.byte	-128
	.byte	-115
	.byte	20
	.byte	32
	.byte	0
	.byte	-127
	.byte	-1
	.byte	-81
	.text
.global	main
	.type	main, @function
main:
	push r28
	push r29
	in r28,__SP_L__
	in r29,__SP_H__
	sbiw r28,14
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
/* prologue: function */
/* frame size = 14 */
/* stack size = 16 */
.L__stack_usage = 16
	ldi r24,lo8(40)
	ldi r25,0
	ldi r18,lo8(39)
	ldi r19,0
	ldi r20,lo8(4)
	movw r30,r18
	st Z,r20
	movw r30,r24
	st Z,r20
	ldi r24,lo8(37)
	ldi r25,0
	ldi r18,lo8(36)
	ldi r19,0
	ldi r20,lo8(44)
	movw r30,r18
	st Z,r20
	movw r30,r24
	st Z,r20
	ldi r24,lo8(76)
	ldi r25,0
	ldi r18,lo8(80)
	movw r30,r24
	st Z,r18
	ldi r24,lo8(77)
	ldi r25,0
	ldi r18,lo8(1)
	movw r30,r24
	st Z,r18
	ldi r24,lo8(40)
	ldi r25,0
	ldi r18,lo8(40)
	ldi r19,0
	movw r30,r18
	ld r18,Z
	andi r18,lo8(-5)
	movw r30,r24
	st Z,r18
	ldi r24,lo8(37)
	ldi r25,0
	ldi r18,lo8(37)
	ldi r19,0
	movw r30,r18
	ld r18,Z
	andi r18,lo8(-5)
	movw r30,r24
	st Z,r18
	ldi r24,lo8(10)
	ldi r30,lo8(.LC0)
	ldi r31,hi8(.LC0)
	movw r26,r28
	adiw r26,5
	0:
	ld r0,Z+
	st X+,r0
	dec r24
	brne 0b
	movw r24,r28
	adiw r24,5
	call bootOLED
	ldi r24,lo8(37)
	ldi r25,0
	ldi r18,lo8(37)
	ldi r19,0
	movw r30,r18
	ld r18,Z
	ori r18,lo8(4)
	movw r30,r24
	st Z,r18
	ldi r24,lo8(40)
	ldi r25,0
	ldi r18,lo8(40)
	ldi r19,0
	movw r30,r18
	ld r18,Z
	ori r18,lo8(4)
	movw r30,r24
	st Z,r18
	ldi r24,lo8(37)
	ldi r25,0
	ldi r18,lo8(37)
	ldi r19,0
	movw r30,r18
	ld r18,Z
	andi r18,lo8(-5)
	movw r30,r24
	st Z,r18
	std Y+2,__zero_reg__
	std Y+1,__zero_reg__
.L10:
	ldi r24,lo8(8)
	std Y+3,r24
	rjmp .L6
.L9:
	ldi r24,lo8(-128)
	std Y+4,r24
	rjmp .L7
.L8:
	ldi r24,lo8(78)
	ldi r25,0
	ldd r18,Y+4
	mov r20,r18
	ldi r21,0
	ldd r18,Y+3
	mov r22,r18
	ldi r23,0
	ldd r18,Y+1
	ldd r19,Y+2
	mov r0,r22
	rjmp 2f
	1:
	lsr r19
	ror r18
	2:
	dec r0
	brpl 1b
	add r18,r20
	adc r19,r21
	andi r18,8
	clr r19
	movw r20,r18
	lsr r21
	ror r20
	lsr r21
	ror r20
	lsr r21
	ror r20
	ldd r18,Y+3
	mov r18,r18
	ldi r19,0
	add r18,r20
	adc r19,r21
	andi r18,lo8(1)
	neg r18
	movw r30,r24
	st Z,r18
	ldd r24,Y+4
	subi r24,lo8(-(-1))
	std Y+4,r24
.L7:
	ldd r24,Y+4
	tst r24
	brne .L8
	ldd r24,Y+3
	subi r24,lo8(-(-1))
	std Y+3,r24
.L6:
	ldd r24,Y+3
	tst r24
	brne .L9
	ldd r24,Y+1
	ldd r25,Y+2
	adiw r24,1
	std Y+2,r25
	std Y+1,r24
	rjmp .L10
	.size	main, .-main
	.ident	"GCC: (GNU) 4.7.2"
.global __do_copy_data
