#include "../kernel/kernel.h"

#define RELEASE "0"
#define VERSION "0.1"

PRIVATE void mm_test(void)
{
    unsigned long i;
    
    i = get_free_page();
    if(!i) panic("No pages in memory");

    ((char *)i)[1] = 7;
    if(((char *)i)[1] != 7)
        panic("Invalid paging");

    free_page(i);
}

EXTERN time_t startup_time;

PUBLIC void main(void)
{    
    trap_init();
    sched_init();
    tty_init();
    time_init();

    sti();

    mm_test();

    printk("\n\nMunix version %s.%s\n", RELEASE, VERSION);

    ASM("call 0x8000");      // run test_util/1.asm file
}

PRIVATE long stack[1024];

// used in boot/head.s
PUBLIC struct {
    long * ptr;
    short segment;
} PACKED stack_start = { &stack[1024], 0x10 };
