#include "lib.h"

size_t strlen(const char * str)
{
    register size_t len = 0;
    while(*str++) 
        len++;
    return len;
}

char * strcpy(char * dest, const char * src)
{
    while(*src)
        *dest++ = *src++;
    return dest;
}
