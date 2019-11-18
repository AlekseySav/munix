#include "kernel.h"

char init_task_stack[PAGE_SIZE];
char task2_stack[PAGE_SIZE];

struct task_struct init_task = {
	{
		0, 
		(long)(&init_task_stack) + PAGE_SIZE, 0x10,
		0, 0, 0, 0,
		(long)&pg_dir, 
		0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0x17, 0x0f, 0x17, 0x17, 0x17, 0x17,
		_LDT(0), 0x80000000
	},
	{
		{0, 0},
		{0x7ff, 0xc0fa00},	// all 8 MiB of memory for init task
		{0x7ff, 0xc0f200}
	}
};

struct task_struct task2 = {
	{
		0, 
		(long)(&task2_stack) + PAGE_SIZE, 0x10,
		0, 0, 0, 0,
		(long)&pg_dir, 
		0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0x17, 0x0f, 0x17, 0x17, 0x17, 0x17,
		_LDT(1), 0x80000000
	},
	{
		{0, 0},
		{0x20000001, 0xc0fa00},
		{0x20000001, 0xc0f200}
	}
};

EXTERN void time_intr(void);

PUBLIC void schedule(void)
{
	task2.tss.eip = 0x1000;			// base (0x2000) + eip (0x1000) = 0x3000
									// 0x3000 is addres of test_util (see boot/head.s)
	set_tss_desc(_TSS(1), &task2.tss);
	set_ldt_desc(_LDT(1), &task2.ldt);
	ASM("ljmp %0, $0" :: "i" (_TSS(1)));
	cli(); while(1);				// that's all the kernel does
	/*
	 * if all is right, kernel will display "test_util... ok" message
	 * two times and something about user mode between them.
	 * so, test util runs two times: in kernel mode and in user mode :-)
	 */
}

PUBLIC void sched_init(void)
{
    set_tss_desc(_TSS(0), &init_task.tss);
	set_ldt_desc(_LDT(0), &init_task.ldt);
	ltr(0);
	lldt(0);

	outb_p(52, 0x43);		        // binary, mode 3, lobyte/hibyte
	outb(LATCH & 0xff, 0x40);	    // lobyte
	outb(LATCH >> 8, 0x40);	        // hibyte

	set_intr_gate(0x20, &time_intr);
	outb(inb_p(0x21) & ~0x01, 0x21);
}
