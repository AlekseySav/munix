#include <stdarg.h>

extern int vsprintf(char * buf, const char * fmt, va_list args);

int sprintf(char * buf, const char * fmt, ...)
{
    int i;
    va_list ap;

    va_start(ap, fmt);
    i = vsprintf(buf, fmt, ap);
    va_end(ap);

    return i;
}
