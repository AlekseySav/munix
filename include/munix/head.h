#ifndef _MUNIX_HEAD_H_
#define _MUNIX_HEAD_H_

struct idt_struct {
    uint16_t low_offset;
    uint16_t segment;
    uint8_t reserved;
    uint8_t flags;
    uint16_t high_offset;
};

struct gdt_struct {
    uint16_t low_limit;
    uint16_t low_base;
    uint8_t high_base;
    uint8_t low_access;
    uint16_t high_limit;
    uint8_t flags;
};

EXTERN struct idt_struct idt[256];
EXTERN struct gdt_struct gdt[256];

#endif
