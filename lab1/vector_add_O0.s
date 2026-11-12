	.arch armv8-a+crc
	.file	"vector_add.c"
	.text
	.align	2
	.global	vector_add_scalar
	.type	vector_add_scalar, %function
vector_add_scalar:
.LFB0:
	.cfi_startproc
	sub	sp, sp, #48
	.cfi_def_cfa_offset 48
	str	x0, [sp, 24]
	str	x1, [sp, 16]
	str	x2, [sp, 8]
	str	x3, [sp]
	str	xzr, [sp, 40]
	b	.L2
.L3:
	ldr	x0, [sp, 40]
	lsl	x0, x0, 2
	ldr	x1, [sp, 24]
	add	x0, x1, x0
	ldr	s30, [x0]
	ldr	x0, [sp, 40]
	lsl	x0, x0, 2
	ldr	x1, [sp, 16]
	add	x0, x1, x0
	ldr	s31, [x0]
	ldr	x0, [sp, 40]
	lsl	x0, x0, 2
	ldr	x1, [sp, 8]
	add	x0, x1, x0
	fadd	s31, s30, s31
	str	s31, [x0]
	ldr	x0, [sp, 40]
	add	x0, x0, 1
	str	x0, [sp, 40]
.L2:
	ldr	x1, [sp, 40]
	ldr	x0, [sp]
	cmp	x1, x0
	bcc	.L3
	nop
	nop
	add	sp, sp, 48
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE0:
	.size	vector_add_scalar, .-vector_add_scalar
	.ident	"GCC: (Debian 14.2.0-19) 14.2.0"
	.section	.note.GNU-stack,"",@progbits
