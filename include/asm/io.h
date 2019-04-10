/*
* asm/io.h
*
* contains definitions for
* working with computer's ports
* (outb, inb, etc)
* for correct work, you should
* include sys/const.h before
* this file (for ASM definition)
*/

#ifndef _ASM_IO_H_
#define _ASM_IO_H_

#ifndef outb
    #define outb(value, port) \
        ASM("outb %%al, %%dx" \
        ::"a" (value), "d" (port))
#endif

#endif
