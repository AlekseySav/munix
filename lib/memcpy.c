#include <stddef.h>

void * memcpy(void * dest, void * src, size_t len)
{
    asm("cld; rep; movsb"
    ::  "D" (dest), 
        "S" (src), 
        "c" (len));
    return dest;
}
