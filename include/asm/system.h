/*
* asm/system.h
*
* contains definitions, which
* are using in munix kernel
* for correct work, you should
* include sys/const.h before
* this file (for ASM definition);
* for correct work idt and gdt
* functions, include munix/head.h
*/

#ifndef _ASM_SYSTEM_H_
#define _ASM_SYSTEM_H_

#ifndef cli
    #define cli() ASM("sti")
#endif

#ifndef sti
    #define sti() ASM("sti")
#endif

#ifndef nop
    #define nop() ASM("nop")
#endif

#endif
