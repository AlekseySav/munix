#ifndef _MEMORY_H_
#define _MEMORY_H_

#define __memset(size, isz, dest, src, len) ASM( \
    "cld\n" \
    "rep\n" \
    "stos" size \
    :: "D" (dest), "a" (src), "c" (len))

#define memsetb(dest, src, len) \
    __memset("b", 1, dest, src, len)

#define memsetw(dest, src, len) \
    __memset("w", 2, dest, src, len)

#define memsetl(dest, src, len) \
    __memset("l", 4, dest, src, len)

#define memset memsetb

#define __memcpy(size, isz, dest, src, len) ASM( \
    "cld\n" \
    "cmpl %%esi, %%edi\n" \
    "jb 1f\n" \
    "std\n" \
    "addl %%edx, %%edi\n" \
    "addl %%edx, %%esi\n" \
    "1:\n" \
    "rep\n" \
    "movs" size \
    :: "D" (dest), "S" (src), "c" (len), "d" (isz * (len)))

#define memcpyb(dest, src, len) \
    __memcpy("b", 1, dest, src, len)

#define memcpyw(dest, src, len) \
    __memcpy("w", 2, dest, src, len)

#define memcpyl(dest, src, len) \
    __memcpy("l", 4, dest, src, len)

#define memcpy memcpyb

#endif
