#include "kernel.h"

PRIVATE const char * trap_errors[] = {
    "Divide error",
	"Debug exception",
	"Nonmaskable interrupt",
	"Breakpoint",
	"Overflow",
	"Bounds check",
	"Invalid opcode",
	"Coprocessor not available",
	"Double fault",
	"Copressor segment overrun",
	"Invalid TSS",
	"Segment not present",
	"Stack exception",
	"General protection",
	"Page fault",
	NULL,
	"Coprocessor error"
};

#define ERRORS_SIZE     (sizeof(trap_errors) / sizeof(void *))

PUBLIC void exception(int nr, int err, int eip, int cs, int eflags)
{
    printk("code = %d\n", err);

    if(nr < 0 || nr > ERRORS_SIZE)
        printk("Catched unrecognized exception %d\n", nr);
    else if(trap_errors[nr] == NULL)
        printk("Catched reserved (15, 17-31) error\n");
    else printk("%s\n", trap_errors[nr]);

    printk("eip=%d\n", eip);
    printk("cs=%d\n", cs);
    printk("eflags=%d\n", eflags);

    panic("exception in system code");
}

// defined in asm.s
EXTERN void divide_error(void);
EXTERN void debug_except(void);
EXTERN void nomask_intr(void);
EXTERN void breakpoint(void);
EXTERN void overflow(void);
EXTERN void bounds_check(void);
EXTERN void invalid_opcode(void);
EXTERN void coprocessor_na(void);
EXTERN void double_fault(void);
EXTERN void coprocessor_overrun(void);
EXTERN void invalid_tss(void);
EXTERN void segment_np(void);
EXTERN void stack_exept(void);
EXTERN void general_protect(void);
EXTERN void page_fault(void);
EXTERN void reserved_trap(void);
EXTERN void coprocessor_error(void);

PUBLIC void trap_init(void)
{
    outb_p(0x11, 0x20);
    outb_p(0x11, 0xa0);

    outb(0x20, 0x21);
    outb(0x28, 0xa1);    
    outb(0x04, 0x21);
    outb(0x02, 0xa1);
    
    outb_p(0x01, 0x21);
    outb_p(0x01, 0xa1);

    set_trap_gate(0, &divide_error);
    set_trap_gate(1, &debug_except);
    set_trap_gate(2, &nomask_intr);
    set_trap_gate(3, &breakpoint);
    set_trap_gate(4, &overflow);
    set_trap_gate(5, &bounds_check);
    set_trap_gate(6, &invalid_opcode);
    set_trap_gate(7, &coprocessor_na);
    set_trap_gate(8, &double_fault);
    set_trap_gate(9, &coprocessor_overrun);
    set_trap_gate(10, &invalid_tss);
    set_trap_gate(11, &segment_np);
    set_trap_gate(12, &stack_exept);
    set_trap_gate(13, &general_protect);
    set_trap_gate(14, &page_fault);
    set_trap_gate(15, &reserved_trap);
    set_trap_gate(16, &coprocessor_error);

    for(int i = 17; i < 32; i++)
        set_trap_gate(i, &reserved_trap);
}
