#include "kernel.h"

EXTERN void time_intr(void);
EXTERN void sys_call(void);

PUBLIC void schedule(int smt, int smt1, int eip)
{
    // switch_to(0);
}

struct tss_struct init_tss = { 0 };

PUBLIC void sched_init(void)
{
    // set_tss_desc(TSS_FIRST_ENTRY, init_tss);
    // ASM("ltr %%ax" :: "a" (TSS_FIRST_ENTRY << 3));

	outb_p(52, 0x43);		        // binary, mode 3, lobyte/hibyte
	outb(LATCH & 0xff, 0x40);	    // lobyte
	outb(LATCH >> 8, 0x40);	        // hibyte

	set_trap_gate(0x20, &time_intr);
	outb(inb_p(0x21) &~ 0x01, 0x21);

    set_intr_gate(0x80, sys_call);
}
