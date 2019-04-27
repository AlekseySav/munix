#include <sys/const.h>
#include <munix/tty.h>
#include <stdarg.h>
#include <string.h>

char buf[TTY_BUF_SIZE];

PUBLIC int printk(const char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    int i = vsprintf(buf, fmt, args);
    tty_write(0, buf);
    va_end(args);
}
