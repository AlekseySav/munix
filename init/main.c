#include <ansi.h>       // for PACKED
#include <sys/const.h>
#include <asm/system.h> // for sti(), nop()
#include <string.h>
#include <munix/tty.h>
#include <munix/kernel.h>   // for printk

PUBLIC void main(void)
{
    tty_init();
    sti();

    printk("System loaded\n");

    while(TRUE);
}

long stack[1024];

// used in boot/head.s
struct {
    long * ptr;
    short segment;
} PACKED stack_start = { &stack[1024], 0x10 };
