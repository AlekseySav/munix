#ifndef _ASM_IO_H_
#define _ASM_IO_H_

#ifndef outb
    #define outb(value, port) asm("outb %%al,%%dx"::"a" (value),"d" (port))
#endif

#ifndef cli
    #define cli() asm("cli")
#endif

#ifndef sti
    #define sti() asm("sti")
#endif

#endif
