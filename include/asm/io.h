#ifndef _ASM_IO_H_
#define _ASM_IO_H_

#define outb(value, port) \
    ASM("outb %%al, %%dx" :: "a" (value), "d" (port))

#endif
