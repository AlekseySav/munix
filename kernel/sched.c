#include <munix/sched.h>
#include <asm/system.h>
#include <asm/io.h>

extern void timer_intr(void);

static char init_stack[4096];

static struct task_struct init_task = {
    /* ldt */ { 
        { 0, 0 },                       // NULL
        { 0x000007ff, 0x00c0fa00 },     // code
        { 0x000007ff, 0x00c0f200 }      // data
    },
    /* tss */ {
        0,
        (long)&init_stack + 4096, 0x10,
        0, 0, 0, 0, 
        (long)&pg_dir, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0x17, 0x0f, 0x17, 0x17, 0x17, 0x17,
        _LDT(0), 0, 0
    }
};

struct task_struct * task_table[NR_TASKS] = { &init_task, 0 };
struct task_struct * current;

static int last = 0;

void shedule(void)
{
    last = (last + 1);

    struct task_struct * next = task_table[last];
    if(next == current)
        return;
    if(last > 1)
        return;

    switch_to(last);
}

void sched_init(void)
{
    set_ldt_desc(FIRST_LDT, &init_task.ldt);        /* NOTE! use FIRS_LDT + (nr << 1); not _LDT(nr) */
    set_tss_desc(FIRST_TSS, &init_task.tss);        /* ... */
    ltr(0);
    lldt(0);
    current = &init_task;


	outb(52, 0x43);		                            // binary, mode 3, lobyte/hibyte
	outb(LATCH & 0xff, 0x40);	                    // lobyte
	outb(LATCH >> 8, 0x40);	                        // hibyte

	set_intr_gate(0x20, &timer_intr);
	outb(inb(0x21) & ~0x01, 0x21);
}
