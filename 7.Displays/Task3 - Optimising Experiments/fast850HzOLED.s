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
	movw r30,r24
	adiw r24,10
.L3:
	ld r18,Z+
	out 0x2e,r18
.L2:
	in __tmp_reg__,0x2d
	sbrs __tmp_reg__,7
	rjmp .L2
	cp r30,r24
	cpc r31,r25
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
	ldi r18,lo8(4)
	out 0x7,r18
	out 0x8,r18
	ldi r18,lo8(44)
	out 0x4,r18
	out 0x5,r18
	ldi r18,lo8(80)
	out 0x2c,r18
	ldi r18,lo8(1)
	out 0x2d,r18
	cbi 0x8,2
	cbi 0x5,2
	ldi r18,lo8(10)
	ldi r30,lo8(.LC0)
	ldi r31,hi8(.LC0)
	movw r26,r28
	adiw r26,1
	0:
	ld r0,Z+
	st X+,r0
	dec r18
	brne 0b
	movw r30,r28
	adiw r30,1
	movw r18,r28
	subi r18,-11
	sbci r19,-1
.L9:
	ld r20,Z+
	out 0x2e,r20
.L8:
	in __tmp_reg__,0x2d
	sbrs __tmp_reg__,7
	rjmp .L8
	cp r30,r18
	cpc r31,r19
	brne .L9
	sbi 0x5,2
	sbi 0x8,2
	cbi 0x5,2
	ldi r26,0
	ldi r27,0
	ldi r16,lo8(-128)
	ldi r17,0
.L13:
	ldi r22,lo8(8)
	ldi r23,0
.L10:
	movw r20,r26
	mov r0,r22
	rjmp 2f
	1:
	lsr r21
	ror r20
	2:
	dec r0
	brpl 1b
	ldi r18,0
	ldi r19,0
	movw r30,r16
	sub r30,r20
	sbc r31,r21
.L11:
	movw r20,r30
	sub r20,r18
	sbc r21,r19
	bst r20,3
	clr r24
	bld r24,0
	ldi r25,0
	add r24,r22
	adc r25,r23
	andi r24,1
	clr r25
	mov r20,r24
	neg r20
	out 0x2e,r20
	subi r18,-1
	sbci r19,-1
	cpi r18,-128
	cpc r19,__zero_reg__
	brne .L11
	subi r22,1
	sbc r23,__zero_reg__
	brne .L10
	adiw r26,1
	rjmp .L13
	.size	main, .-main
	.ident	"GCC: (GNU) 4.7.2"
.global __do_copy_data
