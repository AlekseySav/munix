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

PUBLIC void main(void)
{
    tty_init();
    trap_init();
    sys_init();
    clock_init();
    sched_init();

    sti();

    mm_test();

    printk("\n\033[1;33mMunix version %s.%s\033[0m\n\n", RELEASE, VERSION);

    // for(long * i = pg_dir + 2040; i < pg_dir + 2048; i++)
    //     printk("%x ", *i);

    ASM("call 0x3000"); // run test_util (in boot/head.s)

    // ASM("int3");     // breakpoint
    
    move_to_user_mode();

    ASM("int $0x80" :: "a" (__NR_user),
        "b" ("succesfully switched to user mode\n"));        // well, it is already user call :-)

    // ASM("hlt");         // intel will throw #GP exception
	// ASM VOLATILE("ljmp $0x20, $0");
}

PRIVATE long stack[1024];

// used in boot/head.s
PUBLIC struct {
    long * ptr;
    short segment;
} PACKED stack_start = { &stack[1024], 0x10 };
