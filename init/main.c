/*
 * main.c
 * 
 * this is high-level entry point of kernel.
 * "main" function does nessesary setups and launch INIT process
 */

#include <munix/config.h>
#include <munix/kernel.h>
#include <munix/mm.h>
#include <asm/system.h>
#include <unistd.h>
#include <errno.h>

#define RELEASE "0"
#define VERSION "0.1"

static syscall1(int, write, const char *, msg);

#include <munix/mm.h>

void main(void)         /* NOTE: this is really void, no error here :-) */
{
    tty_init();
    trap_init();
    sys_init();
    sched_init();
    sti();

    printk("\033[1;33mMunix version %s.%s\033[0m\n", RELEASE, VERSION);

    move_to_user_mode();
    
    asm("int3");
    
    if(!fork()) {
        write("1");             // this will be written (by child)
    }
    else {
        write("2");             // this will be written too - but by parent :-)
    }

    asm("1:jmp 1b");
}

static char stack[PAGE_SIZE];

struct {
    char * stack;
    short segment;
} __attribute__((packed)) stack_start = { &stack[PAGE_SIZE], 0x10 };
