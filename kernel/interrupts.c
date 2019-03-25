#include <munix/kernel.h>

struct idt_gate {
    uint16_t low_offset;                            // low 2 bytes of handler addres
    uint16_t segment;                               // gdt code segment (0x08)
    uint8_t reserved;                               // must be 0
    uint8_t flags;                                  // 0x8e - idt gate
    uint16_t high_offset;                           // high 2 bytes of handler addres
};

#define IDT_SIZE 256

EXTERN struct idt_gate idt[IDT_SIZE];               // defined in boot/head.asm
EXTERN void ignore_int(void);                       // also defined in boot/head.asm

PUBLIC void int_set(int n, uint32_t handler)
{
    idt[n].low_offset = (uint16_t)(handler & 0xffff);
    idt[n].segment = 0x08;
    idt[n].reserved = 0;
    idt[n].flags = 0x8E; 
    idt[n].high_offset = (uint16_t)((handler >> 16) & 0xffff);
}

PUBLIC void int_init(void)
{
    cli();
    for(unsigned i = 0; i < IDT_SIZE; i++)
        int_set(i, (uint32_t)ignore_int);
    
    asm(".extern idt_d\n\t");                       // also defined in boot/head.asm
    asm("lidt (idt_d)");

    sti();
}
