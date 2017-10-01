
typedef unsigned int	        uint32_t;
typedef unsigned long long		uint64_t;

#define NULL ((void *)0)

//#define set(R,V)  __asm__ volatile ("MSR " #R ", %0"::"r"(V):"memory")

#define	ioread32(addr) ioread32n(addr,0)
#define	iowrite32(addr, value) iowrite32n(addr,0,value)
#define	ioread32n(addr, idx) ( *((volatile uint32_t*)(addr)+idx) )
#define	iowrite32n(addr, idx, value) do { *((volatile uint32_t*)(addr)+idx) = (value); } while (0)

/* For accessing the system registers */
#define get(V) ({uint64_t _val_;__asm__ volatile ("MRS %0, " #V "" :"=r"(_val_)::"memory");_val_;})
#define set(R,V)  __asm__ volatile ("MSR " #R ", %0; isb"::"r"(V):"memory")
#define seti(R,V) __asm__ volatile ("MSR " #R ", #" #V:::"memory")

/* Barriers */
#define DSB __asm__ volatile("dsb" ::: "memory")
#define ISB __asm__ volatile("isb" ::: "memory")

#define TESTBIT(WORD,X) (WORD & (1ULL << X))

#define GET_BIT(reg, bit) (!!(reg & (1ULL<<bit)))
#define BIT_MASK(a, b) (((unsigned long long) -1 >> (63 - (b))) & ~((1ULL << (a)) - 1))

