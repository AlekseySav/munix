/*
 * trap.c is c-code, that hendles hardware exceptions
 */

#include <munix/config.h>
#include <stddef.h>

#include <munix/kernel.h>
#include <munix/head.h>

#include <asm/io.h>
#include <asm/system.h>

#define TRAPS 32

extern void divide_error(void);
extern void debug_exception(void);
extern void nmi_interrupt(void);
extern void breakpoint(void);
extern void overflow(void);
extern void bound_range_exceeded(void);
extern void invalid_opcode(void);
extern void device_not_available(void);
extern void double_fault(void);
extern void coprocessor_overrun(void);
extern void invalid_tss(void);
extern void segment_not_present(void);
extern void stack_fault(void);
extern void general_protection(void);
extern void page_fault(void);
extern void intel_reserved(void);
extern void math_fault(void);
extern void alignment_check(void);
extern void machine_check(void);
extern void float_exception(void);
extern void virtual_exception(void);

static struct {
    const char * mnemonic;
    const char * name;
} trap_names[] = {
    { "#DE", "Divide error" },
    { "#DB", "Debug exception" },
    { NULL, "NMI interrupt" },
    { "#BP", "Breakpoint" },
    { "#OF", "Overflow" },
    { "#BR", "BOUND Range Exceeded" },
    { "#UD", "Undefined Opcode" },
    { "#NM", "Device Not Available" },
    { "#DF", "Double Fault" },
    { NULL, "Coprocessor Segment Overrun" },
    { "#TS", "Invalid TSS" },
    { "#NP", "Segment Not Present" },
    { "#SS", "Stack Segment Fault" },
    { "#GP", "General Protection" },
    { "#PF", "Page Fault" },
    { NULL, "Intel (15, 21-32) reserved trap" },
    { "#MF", "x87 Floating-Point Error" },
    { "#AC", "Alignment Check" },
    { "#MC", "Machine Check" },
    { "#XM", "SIMD Floating-Point Exception" },
    { "#VE", "Virtualization Exception" }
};

static inline void do_break(long * esp)
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

void exception(unsigned nr, unsigned err, long * esp)
{
    if(nr >= TRAPS)
        printk("Unrecognized exception: %04x\n", err);
    else {
        printk("%s: ", trap_names[nr].name);
        if(trap_names[nr].mnemonic)
            printk("%s %04x\n", trap_names[nr].mnemonic, err);
        else printk("%04x\n", err);
    }

    if(nr == 3) do_break(esp);

    printk("cs:eip\t%04x:%p\n", esp[1], esp[0]);
    printk("ss:esp\t%04x:%p\n", esp[4], esp[3]);
    printk("eflags\t%#x\n", esp[2]);

    if(nr != 3) panic("Exception in system code");
}

void trap_init(void)
{
    outb(0x11, 0x20);
    outb(0x11, 0xa0);

    outb(0x20, 0x21);
    outb(0x28, 0xa1);    
    outb(0x04, 0x21);
    outb(0x02, 0xa1);
    
    outb(0x01, 0x21);
    outb(0x01, 0xa1);

    set_trap_gate(0, &divide_error);
    set_trap_gate(1, &debug_exception);
    set_trap_gate(2, &nmi_interrupt);

    // set_trap_gate(3, &breakpoint);           /* this is better thing */
    set_system_gate(3, &breakpoint);            /* but, i use this for debug */

    set_trap_gate(4, &overflow);
    set_trap_gate(5, &bound_range_exceeded);
    set_trap_gate(6, &invalid_opcode);
    set_trap_gate(7, &device_not_available);
    set_trap_gate(8, &double_fault);
    set_trap_gate(9, &coprocessor_overrun);
    set_trap_gate(10, &invalid_tss);
    set_trap_gate(11, &segment_not_present);
    set_trap_gate(12, &stack_fault);
    set_trap_gate(13, &general_protection);
    set_trap_gate(14, &page_fault);
    set_trap_gate(15, &intel_reserved);
    set_trap_gate(16, &math_fault);
    set_trap_gate(17, &alignment_check);
    set_trap_gate(18, &machine_check);
    set_trap_gate(19, &float_exception);
    set_trap_gate(20, &virtual_exception);

    for(int i = 21; i < TRAPS; i++)
        set_trap_gate(i, &intel_reserved);
}
