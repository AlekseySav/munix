#include <stdarg.h>
#include <munix/kernel.h>
#include <munix/tty.h>

static char buf[TTY_BUF_SIZE];

int printk(const char * fmt, ...)
{
    int i;
    va_list ap;
    va_start(ap, fmt);

    i = vsprintf(buf, fmt, ap);

    asm("pushw %%fs\n\t"
        "pushw %%ds\n\t"
        "popw %%fs\n\t"     // fs = ds
        "pushl %0\n\t"
        "pushl $buf\n\t"
        "pushl $0\n\t"
        "calll tty_write\n\t"
        "addl $8, %%esp\n\t"
        "popl %0\n\t"       // get i
        "popw %%fs"         // restore fs
        :: "r" (i));

    va_end(ap);
    return i;
}
