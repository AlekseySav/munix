/*
 * main.c
 * 
 * this is high-level entry point of kernel.
 * "main" function does nessesary setups and launch INIT process
 */

#include <stddef.h>
#include <munix/kernel.h>
#include <asm/system.h>
#include <string.h>

#define RELEASE "0"
#define VERSION "0.1"

int sys_fork(void);

void main(void)         /* NOTE: this is really void, no error here :-) */
{
    tty_init();
    trap_init();
    sched_init();
    sti();

    printk("\033[1;33mMunix version %s.%s\033[0m\n", RELEASE, VERSION);

    move_to_user_mode();
    
    if(sys_fork()) {
        asm("int3");
        *(short *)0xb8002 = 0x0f30;     // init_task will print this
    }
    else {
        *(short *)0xb8004 = 0x0f30;     // fork_task will prints this :-)
    }

    asm("1:jmp 1b");
}

static char stack[4096];

struct {
    char * stack;
    short segment;
} __attribute__((packed)) stack_start = { &stack[4096], 0x10 };
