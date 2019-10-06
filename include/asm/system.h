#ifndef _ASM_SYSTEM_H_
#define _ASM_SYSTEM_H_

#ifndef cli
    #define cli() ASM("cli")
#endif

#ifndef sti
    #define sti() ASM("sti")
#endif

#ifndef outb
    #define outb(v, buf) ASM("outb %%al, %%dx" :: "a" (v), "d" (buf))
#endif

#endif
