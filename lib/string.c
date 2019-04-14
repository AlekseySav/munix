#include "lib.h"

EXTERN size_t strlen(char * str)
{
    size_t len = 0;
    while(*str++) len++;
    return len;
}

EXTERN int sprintf(char * buf, const char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    return vsprintf(buf, fmt, args);
}
