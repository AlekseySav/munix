#ifndef _MEMORY_H_
#define _MEMORY_H_

#define __memset(size, dest, src, len) ASM( \
    "cld\n" \
    "rep\n" \
    "stos" size \
    :: "D" (dest), "a" (src), "c" (len))

#define memsetb(dest, src, len) \
    __memset("b", dest, src, len)

#define memsetw(dest, src, len) \
    __memset("w", dest, src, len)

#define memsetl(dest, src, len) \
    __memset("l", dest, src, len)

#define memset memsetb

#define __memcpy(size, dest, src, len) ASM( \
    "cld\n" \
    "cmpl %%esi, %%edi\n" \
    "jb 1f\n" \
    "std\n" \
    "addl %%ecx, %%edi\n" \
    "addl %%ecx, %%esi\n" \
    "1:\n" \
    "rep\n" \
    "movs" size \
    :: "D" (dest), "S" (src), "c" (len))

#define memcpyb(dest, src, len) \
    __memcpy("b", dest, src, len)

#define memcpyw(dest, src, len) \
    __memcpy("w", dest, src, len)

#define memcpyl(dest, src, len) \
    __memcpy("l", dest, src, len)

#define memcpy memcpyb

#endif
