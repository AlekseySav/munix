#include "kernel.h"

EXTERN int vsprintf(char * buf, const char * fmt, va_list args);

PRIVATE char buf[TTY_BUF_SIZE];

PUBLIC int printk(const char * fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int i = vsprintf(buf, fmt, ap);

    ASM("push %%fs\n"
        "push %%ds\n"
        "pop %%fs" ::);     // fs = ds
    
    i = tty_write(0, buf, i);
    ASM("pop %%fs" ::);     // restore fs

    va_end(ap);
    return i;
}
