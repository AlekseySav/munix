#include <stddef.h>

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

int memcmp(const void * buf1, const void * buf2, size_t len)
{
    register const char * b1, * b2;
    b1 = buf1;
    b2 = buf2;

    while(len--) {
        if(*b1 != *b2)
            return *b1 - *b2;
        b1++;
        b2++;
    }

    return 0;
}
