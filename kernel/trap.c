#include "kernel.h"

#define TRAPS 32

PRIVATE const char * trap_errors[TRAPS] = {
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
	"Coprocessor error",
    NULL
};

PRIVATE void do_break(long * esp)
{
    short * segments = (short *)(esp - 9);
    long * registers = esp - 7;
    printk("eax\t\tebx\t\tecx\t\tedx\n");
    printk("%08x\t%08x\t%08x\t%08x\n", 
        registers[6], registers[5], registers[4], registers[3]);

    printk("edi\t\tesi\t\tebp\t\tesp\n");
    printk("%08x\t%08x\t%08x\t%08x\n", 
        registers[2], registers[1], registers[0], (long)esp);

    printk("ds\tes\tfs\tgs\n");
    printk("%04x\t%04x\t%04x\t%04x\n\n",
        segments[3], segments[2], segments[1], segments[0]);
}

PUBLIC void exception(long * esp)
{
    int nr = esp[0];
    int err = esp[1];
    
    if(nr < 0 || nr > TRAPS)
        printk("\nCatched unrecognized exception (%d)\n", nr);
    else if(trap_errors[nr] == NULL)
        printk("\nCatched reserved (15, 17-31) error\n");
    else printk("\n%s: %04x\n", trap_errors[nr], err);

    if(nr == 3) do_break(esp);

    printk("cs:eip\t%04x:%p\n", esp[3], esp[2]);
    printk("ss:esp\t%04x:%p\n", esp[6], esp[5]);
    printk("efags\t%#x\n", esp[4]);

    if(nr != 3) panic("exception in system code");
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

    for(int i = 17; i < TRAPS; i++)
        set_trap_gate(i, &reserved_trap);
}
