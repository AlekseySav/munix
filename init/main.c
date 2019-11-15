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
    tty_init();
    trap_init();
    sys_init();
    clock_init();
    sched_init();

    sti();

    mm_test();

    printk("\n\nMunix version %s.%s\n", RELEASE, VERSION);

    // for(long * i = pg_dir + 2040; i < pg_dir + 2048; i++)
    //     printk("%x ", *i);

    ASM("call 0x2000"); // run test_util (in boot/head.s)

    ASM("movl %%esp, %%eax\n\t"
        "pushl $0x17\n\t"
        "pushl %%eax\n\t"
        "pushfl\n\t"
        "pushl $0x0f\n\t"
        "pushl $1f\n\t"
        "iret\n\t"
        "1:"            // now we are in tss[0], ldt[0] (INIT_TASK)
        "movw $0x17, %%ax\n\t"
        "movw %%ax, %%ds\n\t"
        "movw %%ax, %%es\n\t"
        "movw %%ax, %%fs\n\t"
        "movw %%ax, %%gs" ::: "ax");

    // ASM("int $0x80" :: "a" (0));

	// ASM VOLATILE("ljmp $0x20, $0");
}

PRIVATE long stack[1024];

// used in boot/head.s
PUBLIC struct {
    long * ptr;
    short segment;
} PACKED stack_start = { &stack[1024], 0x10 };
