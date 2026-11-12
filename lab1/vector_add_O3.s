	.arch armv8-a+crc
	.file	"vector_add.c"
	.text
	.align	2
	.p2align 4,,15
	.global	vector_add_scalar
	.type	vector_add_scalar, %function
vector_add_scalar:
.LFB3:
	.cfi_startproc
	cbz	x3, .L1
	cmp	x3, 1
	beq	.L12
	add	x5, x0, 4
	add	x4, x1, 4
	sub	x5, x2, x5
	sub	x4, x2, x4
	cmp	x5, 8
	ccmp	x4, 8, 0, hi
	bls	.L12
	sub	x4, x3, #1
	cmp	x4, 2
	mov	x4, 0
	bls	.L4
	lsr	x5, x3, 2
	lsl	x5, x5, 4
	.p2align 3,,7
.L5:
	ldr	q31, [x0, x4]
	ldr	q30, [x1, x4]
	fadd	v30.4s, v31.4s, v30.4s
	str	q30, [x2, x4]
	add	x4, x4, 16
	cmp	x5, x4
	bne	.L5
	tst	x3, 3
	beq	.L1
	and	x4, x3, -4
	sub	x3, x3, x4
	cmp	x3, 1
	beq	.L7
.L4:
	lsl	x5, x4, 2
	ldr	d29, [x0, x5]
	ldr	d28, [x1, x5]
	fadd	v28.2s, v29.2s, v28.2s
	str	d28, [x2, x5]
	tbz	x3, 0, .L1
	and	x3, x3, -2
	add	x4, x4, x3
.L7:
	ldr	s27, [x0, x4, lsl 2]
	ldr	s26, [x1, x4, lsl 2]
	fadd	s26, s27, s26
	str	s26, [x2, x4, lsl 2]
.L1:
	ret
	.p2align 2,,3
.L12:
	mov	x4, 0
	.p2align 3,,7
.L9:
	ldr	s25, [x0, x4, lsl 2]
	ldr	s24, [x1, x4, lsl 2]
	fadd	s24, s25, s24
	str	s24, [x2, x4, lsl 2]
	add	x4, x4, 1
	cmp	x3, x4
	bne	.L9
	ret
	.cfi_endproc
.LFE3:
	.size	vector_add_scalar, .-vector_add_scalar
	.ident	"GCC: (Debian 14.2.0-19) 14.2.0"
	.section	.note.GNU-stack,"",@progbits
