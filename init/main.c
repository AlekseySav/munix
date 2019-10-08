#include "../kernel/kernel.h"

#define RELEASE "0"
#define VERSION "0.1"

void main(void)
{
    tty_init();
    sti();
    
    printk("Munix version %s.%s", RELEASE, VERSION);
}

long stack[1024];

// used in boot/head.s
struct {
    long * ptr;
    short segment;
} PACKED stack_start = { &stack[1024], 0x10 };
