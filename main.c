#include "types.h"
#include "tinyprintf.c"
#include "gicv3.c"
#include "plat.h"

int puts(const char *str)
{
	while (*str) {
		*((unsigned int *) UART_BASE) = *str++;
	}
	return 0;
}

void putc( void* p, char c)
{
	*((unsigned int *) UART_BASE) = c;
}

void info(void)
{
	tfp_printf("CNTFRQ_EL0:%llx\n", get(CNTFRQ_EL0));
	tfp_printf("CNTP_CTL_EL0:%llx\n", get(CNTP_CTL_EL0));
	tfp_printf("CNTP_CVAL_EL0:%llx\n", get(CNTP_CVAL_EL0));
	tfp_printf("CNTP_TVAL_EL0:%llx\n", get(CNTP_TVAL_EL0));
	tfp_printf("CNTPCT_EL0:%llx\n", get(CNTPCT_EL0));
	tfp_printf("MPIDR_EL1:%llx\n", get(MPIDR_EL1));
}

void c_irq_handler()
{
	uint32_t intid = get(S3_0_C12_C12_0) & 0x00FFFFFF; // ICC_IAR1_EL1
	if ( (0 <= intid) && (intid <= 15) ) {
		puts("An SGI\n");
	}
	else {
		puts("TIMER\n");
	}
	puts("IRQ!\n");
	for(;;);
}

void c_exception_handler()
{
	puts("----------\n");
	puts("EXCEPTION!\n\n");
	tfp_printf("SPSR_EL1: %llx\n", get(SPSR_EL1));
	info();

	uint64_t esr = get(ESR_EL1); // Exception syndrome register
	uint64_t ec = esr >> 26;// Exception Class
	tfp_printf("ESR: %llx\n", esr);


	/* Instruction Fault Status Code. This field indicates the type of exception generated */

	if (TESTBIT(esr, 9)) {
		puts("External abort marked as SLVERR");
	}

    if (ec == 0x15) { // syscall
        printf("syscall\n");
        return;
	}

	/* panic();
	/*TODO: ESR_ELn is also updated with a value which
	/*      indicates the cause of the exception
	*/

	puts("Not a syscall...hanging\n");
	for(;;);
}


void enable_timer()
{
	//CNTFRQ_EL0_set(24000000);
	//set(CNTFRQ_EL0, 24000000);
	get(MPIDR_EL1);
}

void main(void)
{
	init_printf(0, putc);

	uint64_t el = get(CurrentEL);

	/* First couple of bits are reserved */
	if (el == (1 << 2)) {
		el = 1;
		tfp_printf("Booting in EL%llu\n", el);
	} else {
		puts("Not EL1 apparently\n");
	}

	info();
	puts("Hello from adrianlshaw\n");
	gic_init();
	puts("Survived GIC initilisation\n");
	puts("Performing SVC call...\n");
	__asm volatile ("SVC #15":::"memory");
	puts("And I'm back to where I was...\n");
	while (1);
}
