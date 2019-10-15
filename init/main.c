#include "../kernel/kernel.h"

#define RELEASE "0"
#define VERSION "0.1"

#define HZ  100
//1193182
#define LATCH (50000 / HZ)

EXTERN void time_intr(void);

PRIVATE void time_init(void)
{
	outb_p(52, 0x43);		        // binary, mode 3, lobyte/hibyte
	outb(LATCH & 0xff, 0x40);	    // lobyte
	outb(LATCH >> 8, 0x40);	        // hibyte

	set_trap_gate(0x20, &time_intr);
	outb(inb_p(0x21) &~ 0x01, 0x21);
}

PUBLIC void main(void)
{
    trap_init();
    time_init();
    sched_init();
    tty_init();

    sti();

    printk("Munix version %s.%s\n", RELEASE, VERSION);
}

PRIVATE long stack[1024];

// used in boot/head.s
PUBLIC struct {
    long * ptr;
    short segment;
} PACKED stack_start = { &stack[1024], 0x10 };
