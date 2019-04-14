/*
* memory.h
*
* contains definitions for working
* with memory (memcpy, memset, etc)
* for correct work, you should
* include sys/const.h before
* this file (for ASM definition)
*/

#ifndef _MEMORY_H_
#define _MEMORY_H_

#ifndef memcpyb
    #define memcpyb(dest, src, len) ASM( \
    "cmp %%edi, %%esi\n\t" \
    "jb 1f\n\t" \
    "cld\n\t" \
    "rep\n\t" \
    "movsb\n\t" \
    "jmp 2f\n\t" \
    "1:\n\t" \
    "addl %%ecx, %%edi\n\t" \
    "addl %%ecx, %%esi\n\t" \
    "std\n\t" \
    "rep\n\t" \
    "movsb\n\t" \
    "2:\n\t" \
    :: "S" (src), "D" (dest), "c" (len))
#endif

#ifndef memcpyw
#define memcpyw(dest, src, len) ASM( \
    "cmp %%edi, %%esi\n\t" \
    "jb 1f\n\t" \
    "cld\n\t" \
    "rep\n\t" \
    "movsw\n\t" \
    "jmp 2f\n\t" \
    "1:\n\t" \
    "addl %%ecx, %%edi\n\t" \
    "addl %%ecx, %%esi\n\t" \
    "std\n\t" \
    "rep\n\t" \
    "movsw\n\t" \
    "2:\n\t" \
    :: "D" (dest), "S" (src), "c" (len))
#endif

#ifndef memsetb
#define memsetb(dest, src, len) ASM( \
    "cld\n\t" \
    "rep\n\t" \
    "stosb\n\t" \
    :: "D" (dest), "a" (src), "c" (len))
#endif

#ifndef memsetw
#define memsetw(dest, src, len) ASM( \
    "cld\n\t" \
    "rep\n\t" \
    "stosw\n\t" \
    :: "D" (dest), "a" (src), "c" (len))
#endif

#ifndef memcpy
    #define memcpy memcpyb
#endif

#ifndef memset
    #define memset memsetb
#endif

#endif
