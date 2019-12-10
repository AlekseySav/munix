#ifndef _ASM_SYSTEM_H_
#define _ASM_SYSTEM_H_

#define cli()   asm("cli")
#define sti()   asm("sti")


#define _set_idt_gate(gate_addr, offset, cs, flags) asm( \
    "movw %%bx, %%ax\n\t" \
    "movw %2, %%bx\n\t" \
    "movl %%eax, %0\n\t" \
    "movl %%ebx, %1\n\t" \
    ::  "m" (*(long *)gate_addr), \
        "m" (*(((long *)gate_addr) + 1)), \
        "i" (flags), \
        "a" (cs << 16), \
        "b" (offset))

#define _idt_flags(p, dpl, d, type) ( \
    (((p) & 1) << 15) | (((dpl) & 3) << 13) | \
    (((d) & 1) << 11) | ((type) << 8))

#define set_intr_gate(n, addr) \
    _set_idt_gate(&idt[n], addr, 0x08, _idt_flags(1, 0, 1, 14))

#define set_trap_gate(n, addr) \
    _set_idt_gate(&idt[n], addr, 0x08, _idt_flags(1, 0, 1, 15))

#define set_system_gate(n, addr) \
    _set_idt_gate(&idt[n], addr, 0x08, _idt_flags(1, 3, 1, 14))


/* NOTE: limit can't exceed 0xffff */
#define _set_gdt_desc(gate_addr, base, limit, flags) do { asm( \
    "movw %%ax, %0\n\t" \
    "shrl $16, %%eax\n\t" \
    "movb %%al, %1\n\t" \
    "movb %%ah, %2\n\t" \
    ::  "m" (*((short *)gate_addr + 1)), \
        "m" (*((char *)gate_addr + 4)), \
        "m" (*((char *)gate_addr + 7)), \
        "a" (base)); \
    *((short *)gate_addr) = limit; \
    *((short *)(((char *)gate_addr) + 5)) = flags; \
    } while(0)

#define _gdt_flags(g, db, p, dpl, s, type) ( \
    (((g) & 1) << 15) | (((db) & 1) << 14) | \
    (((p) & 1) << 7) | (((dpl) & 3) << 5) | \
    (((s) & 1) << 4) | (((type) & 15)))

#define set_ldt_desc(n, addr) \
    _set_gdt_desc(&gdt[n], addr, 24, _gdt_flags(0, 0, 1, 0, 0, 2))

#define set_tss_desc(n, addr) \
    _set_gdt_desc(&gdt[n], addr, 104, _gdt_flags(0, 0, 1, 0, 0, 9))


#define move_to_user_mode() asm( \
    "movl %%esp, %%eax\n\t" \
    "pushl $0x17\n\t" \
    "pushl %%eax\n\t" \
    "pushfl\n\t" \
    "pushl $0x0f\n\t" \
    "pushl $1f\n\t" \
    "iret\n\t" \
    "1:" \
    "movw $0x17, %%ax\n\t" \
    "movw %%ax, %%ds\n\t" \
    "movw %%ax, %%es\n\t" \
    "movw %%ax, %%fs\n\t" \
    "movw %%ax, %%gs" ::: "ax")

#endif
