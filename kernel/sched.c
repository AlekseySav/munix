#include "kernel.h"

struct task_struct init_task = {
	{
		0, 
		(long)(&init_task) + PAGE_SIZE, 0x10,
		0, 0, 0, 0,
		(long)pg_dir, 
		0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0x17, 0x17, 0x17, 0x17, 0x17, 0x17,
		_LDT(0), 0x80000000
	}
};

typedef struct { long a, b; } long2;

long2 ldt[3] = {
	{0, 0},
	{0x9f, 0xc0fa00},
	{0x9f, 0xc0f200}
};

EXTERN void time_intr(void);

PUBLIC void schedule(void)
{
//	ASM("ljmp $0x20, $0");
}

void print_tss(int nr)
{
	nr += TSS_FIRST_ENTRY;
	printk("low_limit: %d\n", gdt[nr].low_limit);
	printk("low_base: %x\n", gdt[nr].low_base);
	printk("mid_base: %x\n", gdt[nr].mid_base);
	printk("access: %x\n", gdt[nr].access);
	printk("flags: %d\n", gdt[nr].flags);
	printk("high_base: %x\n", gdt[nr].high_base);
}

PUBLIC void sched_init(void)
{
    set_tss_desc(_TSS(0), &init_task.tss);
	set_ldt_desc(_LDT(0), &ldt);
	ltr(0);
	lldt(0);

	print_tss(0);

	outb_p(52, 0x43);		        // binary, mode 3, lobyte/hibyte
	outb(LATCH & 0xff, 0x40);	    // lobyte
	outb(LATCH >> 8, 0x40);	        // hibyte

	set_intr_gate(0x20, &time_intr);
	outb(inb_p(0x21) & ~0x01, 0x21);
}
