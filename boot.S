.text

.globl	_start
_start:
	ldr	x4, =0x09000000
	mov	w5, #0x10
	str	w5, [x4, #0x24]
	mov	w5, #0xc300
	orr	w5, w5, #0x0001
	str	w5, [x4, #0x30]

	/* check CPU ID */
	mrs x0, mpidr_el1
	tst x0, #15
	b.ne    slave_cores

bsp:
	/* create the stack */
	adrp x0, stacktop
	mov sp, x0

	ldr x0, =_exc_vector
	msr vbar_el1,x0

	/* Enable GIC */
	isb
	mov	x0, #0x00000007
	msr S3_0_C12_C12_5, x0
	isb
	/* ICC_SRE_EL1 */


	b main

slave_cores:
	b slave_cores


.macro VECTOR_ENTRY func
	.align	7
	b		\func
.endm
	.section .vector
	.global	_exc_vector
	.type	_exc_vector, %function
_exc_vector:
	.align	11
	/**/
	VECTOR_ENTRY	sync_handler
	VECTOR_ENTRY	irq_handler
	VECTOR_ENTRY	fiq_handler
	VECTOR_ENTRY	serr_handler
	/**/
	VECTOR_ENTRY	sync_handler
	VECTOR_ENTRY	irq_handler
	VECTOR_ENTRY	fiq_handler
	VECTOR_ENTRY	serr_handler
	/**/
	VECTOR_ENTRY	sync_handler
	VECTOR_ENTRY	irq_handler
	VECTOR_ENTRY	fiq_handler
	VECTOR_ENTRY	serr_handler
	/**/
	VECTOR_ENTRY	sync_handler
	VECTOR_ENTRY	irq_handler
	VECTOR_ENTRY	fiq_handler
	VECTOR_ENTRY	serr_handler

irq_handler:
	stp		 x0,  x1, [sp, #-16]!
	stp		 x2,  x3, [sp, #-16]!
	stp		 x4,  x5, [sp, #-16]!
	stp		 x6,  x7, [sp, #-16]!
	stp		 x8,  x9, [sp, #-16]!
	stp		x10, x11, [sp, #-16]!
	stp		x12, x13, [sp, #-16]!
	stp		x14, x15, [sp, #-16]!
	stp		x16, x17, [sp, #-16]!
	stp		x18, x30, [sp, #-16]!

	bl		c_irq_handler

	ldp		x18, x30, [sp], #16
	ldp		x16, x17, [sp], #16
	ldp		x14, x15, [sp], #16
	ldp		x12, x13, [sp], #16
	ldp		x10, x11, [sp], #16
	ldp		 x8,  x9, [sp], #16
	ldp		 x6,  x7, [sp], #16
	ldp		 x4,  x5, [sp], #16
	ldp		 x2,  x3, [sp], #16
	ldp		 x0,  x1, [sp], #16
	eret

sync_handler:
fiq_handler:
serr_handler:
exc_handler:
	stp		 x0,  x1, [sp, #-16]!
	stp		 x2,  x3, [sp, #-16]!
	stp		 x4,  x5, [sp, #-16]!
	stp		 x6,  x7, [sp, #-16]!
	stp		 x8,  x9, [sp, #-16]!
	stp		x10, x11, [sp, #-16]!
	stp		x12, x13, [sp, #-16]!
	stp		x14, x15, [sp, #-16]!
	stp		x16, x17, [sp, #-16]!
	stp		x18, x30, [sp, #-16]!

	bl		c_exception_handler

	ldp		x18, x30, [sp], #16
	ldp		x16, x17, [sp], #16
	ldp		x14, x15, [sp], #16
	ldp		x12, x13, [sp], #16
	ldp		x10, x11, [sp], #16
	ldp		 x8,  x9, [sp], #16
	ldp		 x6,  x7, [sp], #16
	ldp		 x4,  x5, [sp], #16
	ldp		 x2,  x3, [sp], #16
	ldp		 x0,  x1, [sp], #16
	eret
