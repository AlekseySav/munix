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
    "movw %%bx, %2\n" \
    "movw %%ax, %%bx\n" \
    "shrl $16, %%eax\n" \
    "shll $16, %%ebx\n" \
    "movw $104, %%bx\n" /* 104 is sizeof tss */ \
    "movl %%ebx, %0\n" \
    "movb %%al, %1\n" \
    "movb %%ah, %3\n" \
    ::  "m" (*(long *)gate_addr), \
        "m" (*(((char *)gate_addr) + 4)), \
        "m" (*(((char *)gate_addr) + 5)), /* actualy, it's short :-) */ \
        "m" (*(((char *)gate_addr) + 7)), \
        "a" (offset), \
        "b" (flags))

#define _access_flags(access, gr, sz) \
    (access) | \
    ((((gr & 0x1) << 7) | ((sz & 0x1) << 6)) << 8)

#define _desc_flags(type) \
    _access_flags(type, 0, 0)

#define set_tss_desc(n, addr) \
    _set_desc(((char *)gdt + n), _desc_flags(0x89), addr)

#define set_ldt_desc(n, addr) \
    _set_desc(((char *)gdt + n), _desc_flags(0x82), addr)

#endif
