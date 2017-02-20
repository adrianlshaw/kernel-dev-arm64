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

	b main

slave_cores:
	b slave_cores
