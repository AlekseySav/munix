.globl start, pg_dir, idt, gdt

#include <munix/config.h>

/*
 * boot/head.S
 *
 * This is 32 bit startup of kernel
 * Start of head.S is also start of memory.
 * And it is start of page directory (pg_dir).
 * NOTE, that all following code will be eraesed and set as paging.
 */

.text

pg_dir:                                     /* page directory */
start:                                      /* entry */
    xorl %eax, %eax
    movl %eax, 0x00100000
    incl %eax
    movl %eax, 0x00000000                   /* check, A20 line is really enabled */
    cmpl %eax, 0x00100000
1:  je 1b

    movl $__bss_start, %edi
    movl $__bss_end, %ecx
    subl %edi, %ecx
    shrl $2, %ecx
    xorl %eax, %eax
    cld
rep stosl                                   /* zero bss section */

    movl %cr0, %eax
    andl $0x80000011, %eax                  /* save PE, PG, ET only */
    testl $0x10, %eax
    jnz 1f
    orl $0x04, %eax                         /* set EM if ET is not set */
1:  movl %eax, %cr0
    lgdt gdt_d
    ljmp $0x08, $go                         /* far jump to reload cs, etc */
go:
    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    lssl stack_start, %esp
setup_idt:
    movl $ignore_int, %edx
    movl $0x00080000, %eax                  /* cs = 0x08 */
    movw %dx, %ax
    movw $0x8e00, %dx                       /* present = 1, dpl = 00, interrupt_gate */
    movl $255 * 8, %edi
1:  movl %eax, idt(%edi)
    movl %edx, 4 + idt(%edi)
    subl $8, %edi
    jnz 1b
    lidt idt_d

    jmp setup_paging

.org 0x1000
pg0:
.org 0x1000 + [HIGH_MEMORY / 1024]

setup_paging:
    movl $pg0 + 7, %eax                     /* present, user, r/w bits */
    movl $pg_dir, %edi
    movl $HIGH_MEMORY / 0x400000, %ecx
1:  stosl                                   /* set pg_dir */
    addl $4, %edi
    decl %ecx
    jnz 1b
    movl $1024 - [HIGH_MEMORY / 0x400000], %ecx
rep stosl
    
	movl $7, %eax                           /* ... */
	movl $pg0, %edi
	movl $HIGH_MEMORY / 4096, %ecx          /* pages describes this memory */
1:	stosl
	addl $4096, %eax
	decl %ecx
	jnz 1b

    movl $pg_dir, %eax
    movl %eax, %cr3                         /* set pg_dir */

    movl %cr4, %eax
    andl $0x3ff7ce, %eax                    /* unset PAE, PSE bits, etc. */
    movl %eax, %cr4

    movl %cr0, %eax
    orl $0x80000000, %eax
    movl %eax, %cr0                         /* set PG bit */
    
    call main
1:  jmp 1b


ignore_int:                                 /* default interrupt handler :-) */
    incb 0xb8000
    movb $2, 0xb8001
    iret


.align 2
.short 0
gdt_d:
    .short 256 * 8 - 1
    .long gdt

.align 2
.short 0
idt_d:
    .short 256 * 8 - 1
    .long idt

.align 8
gdt:
    .quad 0x0000000000000000                /* gdt same as in boot.S */
    .quad 0x00c09a00000007ff                /* exec/read */
    .quad 0x00c09200000007ff                /* read/write */
    .quad 0x0000000000000000                /* unused (temporary) */
    .fill 252, 8, 0                         /* for tss, ldt, ect. */

idt:
    .fill 256, 8, 0
