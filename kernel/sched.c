#include "kernel.h"

PUBLIC void schedule(int smt, int smt1, int eip)
{
    // switch_to(0);
}

struct tss_struct init_tss = { 0 };

PUBLIC void sched_init(void)
{
    // set_tss_desc(TSS_FIRST_ENTRY, init_tss);
    // ASM("ltr %%ax" :: "a" (TSS_FIRST_ENTRY << 3));
}
