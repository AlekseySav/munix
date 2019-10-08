#include "../kernel/kernel.h"

#define RELEASE "0"
#define VERSION "0.1"

#define HZ  100

#define LATCH (1193182/HZ)

EXTERN void time_intr(void);

PRIVATE void time_init(void)
{
	outb_p(0x36, 0x43);		/* binary, mode 3, LSB/MSB, ch 0 */
	outb(LATCH & 0xff, 0x40);	/* LSB */
	outb(LATCH >> 8, 0x40);	/* MSB */

	set_int_gate(0x20, &time_intr);
	outb(inb_p(0x21) &~ 0x01, 0x21);
}

PRIVATE INLINE void trap_init(void) {
    outb_p(0x11, 0x20);
    outb_p(0x11, 0xa0);

    outb(0x20, 0x21);
    outb(0x28, 0xa1);
    
    outb(0x04, 0x21);
    outb(0x02, 0xa1);
    
    outb_p(0x01, 0x21);
    outb_p(0x01, 0xa1);
}

PUBLIC void main(void)
{
    trap_init();
    time_init();
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
