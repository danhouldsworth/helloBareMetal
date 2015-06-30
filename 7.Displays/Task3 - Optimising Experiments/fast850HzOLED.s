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
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	movw r18,r24
	subi r18,-10
	sbci r19,-1
.L3:
	movw r30,r24
	ld r20,Z+
	movw r24,r30
	out 0x2e,r20
.L2:
	in __tmp_reg__,0x2d
	sbrs __tmp_reg__,7
	rjmp .L2
	cp r24,r18
	cpc r25,r19
	brne .L3
/* epilogue start */
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
	.section	.text.startup,"ax",@progbits
.global	main
	.type	main, @function
main:
	push r28
	push r29
	in r28,__SP_L__
	in r29,__SP_H__
	sbiw r28,10
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
/* prologue: function */
/* frame size = 10 */
/* stack size = 12 */
.L__stack_usage = 12
	ldi r24,lo8(4)
	out 0x7,r24
	out 0x8,r24
	ldi r24,lo8(44)
	out 0x4,r24
	out 0x5,r24
	ldi r24,lo8(80)
	out 0x2c,r24
	ldi r24,lo8(1)
	out 0x2d,r24
	cbi 0x8,2
	cbi 0x5,2
	ldi r24,lo8(10)
	ldi r30,lo8(.LC0)
	ldi r31,hi8(.LC0)
	movw r26,r28
	adiw r26,1
	0:
	ld r0,Z+
	st X+,r0
	dec r24
	brne 0b
	movw r24,r28
	adiw r24,1
	call bootOLED
	sbi 0x5,2
	sbi 0x8,2
	cbi 0x5,2
	ldi r18,0
	ldi r19,0
.L10:
	ldi r24,0
	ldi r25,0
	rjmp .L7
.L8:
	movw r22,r20
	add r22,r30
	adc r23,__zero_reg__
	bst r22,3
	clr r16
	bld r16,0
	ldi r17,0
	add r16,r24
	adc r17,r25
	andi r16,1
	clr r17
	mov r22,r16
	neg r22
	out 0x2e,r22
	subi r30,lo8(-(1))
	cpi r30,lo8(-128)
	brne .L8
	adiw r24,1
	cpi r24,8
	cpc r25,__zero_reg__
	breq .L9
.L7:
	movw r20,r18
	mov r0,r24
	rjmp 2f
	1:
	lsr r21
	ror r20
	2:
	dec r0
	brpl 1b
	ldi r30,0
	rjmp .L8
.L9:
	subi r18,-1
	sbci r19,-1
	rjmp .L10
	.size	main, .-main
	.ident	"GCC: (GNU) 4.7.2"
.global __do_copy_data
