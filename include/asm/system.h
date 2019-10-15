#ifndef _ASM_SYSTEM_H_
#define _ASM_SYSTEM_H_

#ifndef cli
    #define cli() ASM("cli")
#endif

#ifndef sti
    #define sti() ASM("sti")
#endif

#define _set_gate(gate_addr, flags, offset) ASM( \
    "movw %%bx, %%ax\n" \
    "movw %2, %%bx\n" \
    "movl %%eax, %0\n" \
    "movl %%ebx, %1\n" \
    ::  "m" (*(long *)gate_addr), \
        "m" (*(((long *)gate_addr) + 1)), \
        "i" (flags << 8), \
        "a" (0x00080000), \
        "b" (offset))

#define _type_attr(p, dpl, s, type) \
    (((p & 0x1) << 7) | ((dpl & 0x3) << 5) | \
    ((s & 0x1) << 4) | ((type & 0xf)))

#define _intr_flags(dpl, type) \
    _type_attr(1, dpl, 0, type)

#define set_intr_gate(n, addr) \
    _set_gate(&idt[n], _intr_flags(0, 14), addr)

#define set_trap_gate(n, addr) \
    _set_gate(&idt[n], _intr_flags(0, 15), addr)

#define _set_desc(gate_addr, flags, offset) ASM( \
    "movw %%cx, %0\n" \
    "movw %%ax, %1\n" \
    "shrl $16, %eax\n" \
    "movb %%al, %2\n" \
    "movw %%bx, %3\n" \
    "movb %%ah, %4\n" \
    ::  "m" (*(short *)gate_addr), \
        "m" (*(((short *)gate_addr) + 1)), \
        "m" (*(((char *)gate_addr) + 4)), \
        "m" (*(((char *)gate_addr) + 5)), \
        "m" (*(((char *)gate_addr) + 7)), \
        "a" (offset), \
        "b" (flags), \
        "c" (104)) /* 104 is sizeof tss_struct */

#define _access_flags(a, f) (((f) << 8) | ((a)))

#define set_tss_desc(n, addr) \
    _set_desc(&gdt[n], _access_flags(0x89, 0), addr)

#endif
