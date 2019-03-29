#ifndef _STRING_H_
#define _STRING_H_

#include <munix/asm.h>

#ifndef memsetb
    #define memsetb(dest, src, len) \
        if(len >= 0) \
            ASM("cld\n\t" \
                "rep\n\t" \
                "stosb" \
                :: "D" (dest), "a" (src), "c" (len)); \
        else \
            ASM("std\n\t" \
                "rep\n\t" \
                "stosb" \
                :: "D" (dest), "a" (src), "c" (-len))
#endif

#ifndef memsetw
    #define memsetw(dest, src, len) \
        if(len >= 0) \
            ASM("cld\n\t" \
                "rep\n\t" \
                "stosw" \
                :: "D" (dest), "a" (src), "c" (len)); \
        else \
            ASM("std\n\t" \
                "rep\n\t" \
                "stosw" \
                :: "D" (dest), "a" (src), "c" (-len))
#endif

#define memset memsetb

#ifndef memcpyb
    #define memcpyb(dest, src, len) \
        if(len >= 0) \
            ASM("cld\n\t" \
                "rep\n\t" \
                "movsb" \
                :: "D" (dest), "S" (src), "c" (len)); \
        else \
            ASM("std\n\t" \
                "rep\n\t" \
                "movsb" \
                :: "D" (dest), "S" (src), "c" (-len))
#endif

#ifndef memcpyw
    #define memcpyw(dest, src, len) \
        if(len >= 0) \
            ASM("cld\n\t" \
                "rep\n\t" \
                "movsw" \
                :: "D" (dest), "S" (src), "c" (len)); \
        else \
            ASM("std\n\t" \
                "rep\n\t" \
                "movsw" \
                :: "D" (dest), "S" (src), "c" (-len))
#endif

#define memcpy memcpyb

#endif
