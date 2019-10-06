#include <ansi.h>
#include <munix/config.h>
#include <sys/const.h>
#include <sys/types.h>
#include <stddef.h>
#include <stdarg.h>

#include <asm/system.h>

EXTERN int vsprintf(char * buf, const char * fmt, va_list args);

int sprintf(char * buf, const char * fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int i = vsprintf(buf, fmt, ap);
    va_end(ap);
    return i;
}


EXTERN void con_init(void);
EXTERN void con_write(const char * str);

char buf[1024];

void main(void)
{
    con_init();
    
    sprintf(buf, "Munix version %s.%s\n\r", "0", "0.1");

    con_write(buf);
}

long stack[1024];

// used in boot/head.s
struct {
    long * ptr;
    short segment;
} PACKED stack_start = { &stack[1024], 0x10 };
