/*
 * main.c
 * 
 * this is high-level entry point of kernel.
 * "main" function does nessesary setups and launch INIT process
 */

#include <munix/config.h>
#include <stdarg.h>
#include <munix/kernel.h>
#include <munix/mm.h>
#include <asm/system.h>
#include <unistd.h>
#include <errno.h>

#define RELEASE "0"
#define VERSION "0.1"

syscall0(int, setup);
syscall3(int, write, int, fd, char *, buf, int, nr);
syscall0(int, getpid);
syscall1(int, alarm, long, seconds);
syscall0(int, pause);
syscall0(int, getppid);

void init(void);

static char print_buf[1024];

static int printf(const char * fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    return write(1, print_buf, vsprintf(print_buf, fmt, ap));
}

void main(void)         /* NOTE: this is really void, no error here :-) */
{
    tty_init();
    trap_init();
    sys_init();
    sched_init();
    sti();

    printk("\033[1;33mMunix version %s.%s\033[0m\n", RELEASE, VERSION);

    move_to_user_mode();
    
    if(!fork()) {
        init();
    }

    asm("1:jmp 1b");
}

void init(void)
{
    alarm(1);
    pause();
    setup();
    printf("pid: %d\nparent: %d\n", getpid(), getppid());
}

static char stack[PAGE_SIZE];

struct {
    char * stack;
    short segment;
} __attribute__((packed)) stack_start = { &stack[PAGE_SIZE], 0x10 };
