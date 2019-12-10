#ifndef _ASM_IO_H_
#define _ASM_IO_H_

#define outb(value, port) asm( \
    "outb %%al, %%dx" \
        ::  "a" (value), "d" (port) \
        )

#define inb(port) ({ \
    unsigned char value; \
    asm("inb %%dx, %%al" \
        : "=a" (value) : "d" (port)); \
    value; })


#endif
