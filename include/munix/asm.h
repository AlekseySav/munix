#ifndef _MUNIX_ASM_H_
#define _MUNIX_ASM_H_

#include <sys/const.h>

#ifndef outb
    #define outb(value, port) ASM("outb %%al,%%dx"::"a" (value),"d" (port))
#endif

#ifndef inb
#define inb(port) ({ \
    unsigned char _v; \
    ASM VOLATILE ("inb %%dx, %%al"::"a" (_v), "d" (port)); \
    _v; })
#endif

#ifndef cli
    #define cli() ASM("cli")
#endif

#ifndef sti
    #define sti() ASM("sti")
#endif

#endif
