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

#define _int_flags(dpl, type) \
    _type_attr(1, dpl, 0, type)

#define set_int_gate(n, addr) \
    _set_gate(&idt[n], _int_flags(0, 14), addr)

#define set_trap_gate(n, addr) \
    _set_gate(&idt[n], _int_flags(0, 15), addr)

#endif
