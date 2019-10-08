#ifndef _ASM_IO_H_
#define _ASM_IO_H_

#define outb(value, port) \
    ASM("outb %%al, %%dx" \
        :: "a" (value), "d" (port))

#define inb(port) ({ \
    unsigned char value; \
    ASM("inb %%dx, %%al" \
        : "=a" (value) : "d" (port)); \
    value; })

#define outb_p(value, port) \
        ASM VOLATILE("outb %%al, %%dx\n" \
        ".word 0x00eb, 0x00eb" \
        :: "a" (value), "d" (port)); \

#define inb_p(port) ({ \
    unsigned char value; \
    ASM VOLATILE("inb %%dx, %%al\n" \
        ".word 0x00eb, 0x00eb" \
        : "=a" (value) : "d" (port)); \
    value; })

#endif
