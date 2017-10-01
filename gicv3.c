#include "types.h"
#include "plat.h"

#define	GIC_VLPI_BASE			(GIC_GICR_BASE+0x10000u)
#define	GIC_SGI_BASE			(GIC_GICR_BASE+0x10000u)

/* Global distributor */
#define	GICD_CTLR			(GIC_GICD_BASE+0x0000u)		/* RW See the register description Distributor Control Register */
#define	GICD_TYPER			(GIC_GICD_BASE+0x0004u)		/* RO IMPLEMENTATION DEFINED Interrupt Controller Type Register */
#define	GICD_IIDR			(GIC_GICD_BASE+0x0008u)		/* RO IMPLEMENTATION DEFINED Distributor Implementer Identification Register */
#define	GICD_STATUSR			(GIC_GICD_BASE+0x0010u)		/* RW 0x000 00000 Error Reporting Status Register, optional */
#define	GICD_SETSPI_NSR			(GIC_GICD_BASE+0x0040u)		/* WO - Set SPI Register */
#define	GICD_CLRSPI_NSR			(GIC_GICD_BASE+0x0048u)		/* WO - Clear SPI Register */
#define	GICD_SETSPI_SR			(GIC_GICD_BASE+0x0050u)		/* WO - Set SPI, Secure Register */
#define	GICD_CLRSPI_SR			(GIC_GICD_BASE+0x0058u)		/* WO - Clear SPI, Secure Register */
#define	GICD_IGROUPR	 		(GIC_GICD_BASE+0x0080u)		/* RW IMPLEMENTATION DEFINED Interrupt Group Registers */
#define	GICD_ISENABLER	 		(GIC_GICD_BASE+0x0100u)		/* RW IMPLEMENTATION DEFINED Interrupt Set-Enable Registers */
#define	GICD_ICENABLER	 		(GIC_GICD_BASE+0x0180u)		/* RW IMPLEMENTATION DEFINED Interrupt Clear-Enable Registers */
#define	GICD_ISPENDR	 		(GIC_GICD_BASE+0x0200u)		/* RW 0x0000 0000 Interrupt Set-Pending Registers */
#define	GICD_ICPENDR	 		(GIC_GICD_BASE+0x0280u)		/* RW 0x0000 0000 Interrupt Clear-Pending Registers */
#define	GICD_ISACTIVER	 		(GIC_GICD_BASE+0x0300u)		/* RW 0x0000 0000 Interrupt Set-Active Registers */
#define	GICD_ICACTIVER	 		(GIC_GICD_BASE+0x0380u)		/* RW 0x0000 0000 Interrupt Clear-Active Registers */
#define	GICD_IPRIORITYR	 		(GIC_GICD_BASE+0x0400u)		/* RW 0x0000 0000 Interrupt Priority Registers */
#define	GICD_ITARGETSR		 	(GIC_GICD_BASE+0x0800u)		/* RO IMPLEMENTATION DEFINED Interrupt Processor Targets Registers */
#define	GICD_ICFGR			(GIC_GICD_BASE+0x0C00u)		/* RW IMPLEMENTATION DEFINED Interrupt Configuration Registers */
#define	GICD_IGRPMODR			(GIC_GICD_BASE+0x0D00u)		/* - 0x0000 0000 Interrupt Group Modifier Registers */
#define	GICD_NSACR			(GIC_GICD_BASE+0x0E00u)		/* RW 0x0000 0000 Non-secure Access Control Registers */
#define	GICD_SGIR			(GIC_GICD_BASE+0x0F00u)		/* WO - Software Generated Interrupt Register */
#define	GICD_CPENDSGIR			(GIC_GICD_BASE+0x0F10u)		/* RW 0x0000 0000 SGI Clear-Pending Registers */
#define	GICD_SPENDSGIR			(GIC_GICD_BASE+0x0F20u)		/* RW 0x0000 0000 SGI Set-Pending Registers */
#define	GICD_IROUTER			(GIC_GICD_BASE+0x6000u)		/* RW 0x0000 0000 Interrupt Routing Registers */

/* Local redistributor */
#define	GICR_IGROUPR0			(GIC_SGI_BASE+0x0080u)		/* RW - Interrupt Group Register 0 */
#define	GICR_ISENABLER0			(GIC_SGI_BASE+0x0100u)		/* RW IMPLEMENTATION DEFINED Interrupt Set-Enable Register 0 */
#define	GICR_ICENABLER0			(GIC_SGI_BASE+0x0180u)		/* RW IMPLEMENTATION DEFINED Interrupt Clear-Enable Register 0 */
#define	GICR_ISPENDR0			(GIC_SGI_BASE+0x0200u)		/* RW 0x0000 0000 Interrupt Set-Pend Register 0 */
#define	GICR_ICPENDR0			(GIC_SGI_BASE+0x0280u)		/* RW 0x0000 0000 Interrupt Clear-Pend Register 0 */
#define	GICR_ISACTIVER0			(GIC_SGI_BASE+0x0300u)		/* RW 0x0000 0000 Interrupt Set-Active Register 0 */
#define	GICR_ICACTIVER0			(GIC_SGI_BASE+0x0380u)		/* RW 0x0000 0000 Interrupt Clear-Active Register 0 */
#define	GICR_IPRIORITYR			(GIC_SGI_BASE+0x0400u)		/* RW 0x0000 0000 Interrupt Priority Registers */
#define	GICR_ICFGR0			(GIC_SGI_BASE+0x0C00u)		/* RW IMPLEMENTATION DEFINED SGI Configuration Register */
#define	GICR_ICFGR1			(GIC_SGI_BASE+0x0C04u)		/* RW IMPLEMENTATION DEFINED PPI Configuration Register */
#define	GICR_IGRPMODR0			(GIC_SGI_BASE+0x0D00u)		/* RW - Interrupt Group Modifier Register 0 */
#define	GICR_NSACR			(GIC_SGI_BASE+0x0E00u)		/* RW 0x0000 0000 Non-Secure Access Control Register */

#define	GICR_CTLR			(GIC_GICR_BASE+0x0000u) /* RW See the register description Redistributor Control Register */
#define	GICR_WAKER			(GIC_GICR_BASE+0x0014u) /* RW See the register description Redistributor Wake Register */
#define	GICR_STATUSR			(GIC_GICR_BASE+0x0010u) /* RW 0x0000 0000 Error Reporting Status Register, optional */


#define TIMER_VALUE 0x500000u // amount of time to perform a timer tick


void wait(uint32_t t)
{
	volatile uint32_t count = t;
	while ( 0 < count ) {
		count--;
	}
}

void enable_interrupts(void)
{
	 __asm__ volatile("msr daifclr, #3" ::: "memory");
}

void disable_interrupts(void)
{
	__asm__ volatile("msr daifset, #3" ::: "memory");
}

void timer_test(void)
{
	/* SGI enable */
	iowrite32(GICR_ISENABLER0, 0x4000ffff);

	/* timer init */
	set(CNTP_CTL_EL0,0x00000000);  // Disable timer
	set(CNTP_TVAL_EL0,TIMER_VALUE); // Set timer value
	set(CNTP_CTL_EL0,0x00000001);  // Enable timer


/*
	disable_interrupts();

	for (uint32_t ix=0;; ix++) {
		set(S3_0_C12_C11_5,(0x000000000000FFFFL | ((ix&0xf) << 24))); // ICC_SGI1R_EL1
		wait(5000000);
		tfp_printf("Arming the timer\n");
	}
*/
}

/* Check register bit to ensure that old settings are no longer visible to cores */
static void gicd_wait(void)
{
	while (ioread32(GICD_CTLR) & (0x01<<31));
}

/* Check register bit to ensure that old settings are no longer visible to cores */
static void gicr_wait(void)
{
	while (ioread32(GICD_CTLR) & (0x01<<3));
}

/* Main core only */
static void _enable_distributor(void)
{
	int ix;
	uint32_t intr_lines = ioread32(GICD_TYPER);

	intr_lines = 32 * ((intr_lines & 0x1f) + 1);
	if ( 1021 < intr_lines ) {
		intr_lines = 1021;
	}

	tfp_printf("GICD_TYPER=%08X\n", intr_lines);
	tfp_printf("INTR_LINES=%d\n", intr_lines);

	/* GICD control disable */
	iowrite32(GICD_CTLR, 0x00000030);

	gicd_wait();

	/* SPI GROUP (Secure Group 1) */
	for ( ix = 1; ix < ((intr_lines+31)/32); ix++ ) {
		iowrite32n(GICD_IGROUPR, ix, 0x00000000);
		iowrite32n(GICD_IGRPMODR, ix, 0xffffffff);
	}
	/* SPI Clear-Enable */
	for ( ix = 1; ix < ((intr_lines+31)/32); ix++ ) {
		iowrite32n(GICD_ICENABLER, ix, 0xffffffff);
	}

	gicd_wait();

	/* SPI Clear-Pending */
	for ( ix = 1; ix < ((intr_lines+31)/32); ix++ ) {
		iowrite32n(GICD_ICPENDR, ix, 0xffffffff);
	}
	/* SPI Clear-Active */
	for ( ix = 1; ix < ((intr_lines+31)/32); ix++ ) {
		iowrite32n(GICD_ICACTIVER, ix, 0xffffffff);
	}
	/* SPI Priority */
	for ( ix = 8; ix < ((intr_lines+3)/4); ix++ ) {
		iowrite32n(GICD_IPRIORITYR, ix, 0x00000000);
	}
	/* SPI Configuration (Trigger-Mode) */
	for ( ix = 2; ix < ((intr_lines+15)/16); ix++ ) {
		iowrite32n(GICD_ICFGR, ix, 0x00000000);
	}
	/* SPI NonSecure Access Control */
	for ( ix = 2; ix < ((intr_lines+15)/16); ix++ ) {
		iowrite32n(GICD_NSACR, ix, 0x00000000);
	}
	/* SPI Routing (Aff=CORE0) */
	for ( ix = 32; ix < intr_lines; ix++ ) {
		iowrite32n(GICD_IROUTER, ix, 0x00000000);
	}
	/* GICD control enable */
	iowrite32(GICD_CTLR, 0x00000037);

	gicd_wait();

}

static void _enable_redistributor(void)
{
	int ix;

	iowrite32(GICR_STATUSR, 0xf);

	iowrite32(GICR_WAKER, ioread32(GICR_WAKER) & ~(0x01<<1));
	while (ioread32(GICR_WAKER) & (0x01<<2));


	iowrite32(GICR_CTLR, 0x00000000);

	gicr_wait();

	iowrite32(GICR_IGROUPR0, 0x00000000);
	iowrite32(GICR_IGRPMODR0, 0xffffffff);
	iowrite32(GICR_ICENABLER0, 0xffffffff);

	gicr_wait();

	iowrite32(GICR_ICPENDR0, 0xffffffff);
	iowrite32(GICR_ICACTIVER0, 0xffffffff);

	for ( ix = 0; ix < 8; ix++ ) {
		iowrite32n(GICR_IPRIORITYR, ix, 0x00000000);
	}

	iowrite32(GICR_ICFGR0, 0x00000000);
	iowrite32(GICR_ICFGR1, 0x00000000);
	iowrite32(GICR_NSACR,  0x00000000);
}

/* CPU interface */
static void _enable_cpu_interface(void)
{	
	set(S3_0_C12_C12_4, 0x00000000); // ICC_CTLR_EL1
	set(S3_0_C12_C8_3,0); // ICC_BPR0_EL1
	set(S3_0_C12_C12_7,0); // ICC_BPR1_EL1
	set(S3_0_C4_C6_0, 0xff); // ICC_PMR_EL1
	set(S3_0_C12_C8_3, 0x00000001); // ICC_IGRPEN1_EL1
}

/* Detect number of interrupt lines and initialise */
void gic_init()
{
	_enable_distributor();

	_enable_redistributor();

	_enable_cpu_interface();

}
