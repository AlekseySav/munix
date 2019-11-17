/*
* head.s 
*
* contains 32-bit startup code of kernel
*/

.globl start, idt, gdt, pg_dir

/*
 * page directory entry
 * it is set by setup_paging routine
 */
pg_dir:

/*
 * this is the startup kernel code
 * but it would be cleared and set as page directory
 */
start:
	lss stack_start, %esp									/* setup stack */

    call setup_idt
    call setup_gdt

    movl $0x10, %eax                                        /* load all segments to 0x10 */
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
	lss stack_start, %esp

    xorl %eax,%eax
1:	incl %eax		                                        /* check that A20 really IS enabled */
	movl %eax,0x000000
	cmpl %eax,0x100000
	je 1b

	movl %cr0, %eax		                                    /* check math chip */
	andl $0x80000011, %eax	                                /* Save PG,ET,PE */
	testl $0x10, %eax
	jne 1f			                                        /* ET is set - 387 is present */
	orl $4, %eax		                                    /* else set emulate bit */
1:	movl %eax, %cr0

    jmp afterpg

/*
* setup_idt
*
* sets all interrupt gates of idt as
* pointers to ignore_int functions
*/
setup_idt:
	lea ignore_int, %edx
	movl $0x00080000, %eax
	movw %dx, %ax		                                    /* selector = 0x0008 = cs */
	movw $0x8e00, %dx	                                    /* interrupt gate - dpl=0, present */

	lea idt, %edi
	mov $256, %ecx                                          /* size of idt */
rp_sidt:
	movl %eax, (%edi)
	movl %edx, 4(%edi)
	addl $8, %edi
	dec %ecx
	jne rp_sidt
	lidt idt_d
	ret

/*
* setup_gdt
*
* this routine just only loads
* gdt descriptor (using lgdt)
* and reloads cs (by ret command)
*/
setup_gdt:
    lgdt gdt_d
    ret

/*
 * this is page tables for pg_dir
 * all previos code would be overwritten for paging
 * we use 8 MiB of memory (see gdt belong ...)
 * so, to describe it, we need too page_tables (pg0 and pg1)
 * ... as one page table describes 4MiB
*/
.org 0x1000
pg0:
.org 0x2000
pg1:
.org 0x3000
/*
 * this code i copied from test_util/1
 * because it's much esear to run it
 * run it with "call 0x3000"
 */
	xorl %eax, %eax
	int $0x80
	ret

afterpg:
	call setup_paging
    call main
1:  jmp 1b

setup_paging:
	xorl %eax, %eax
	xorl %edi, %edi
	movl $2048, %ecx										/* 8KiB: pg_dir and pg0 */
	cld
rep stosl
	movl $pg0 + 7, pg_dir									/* user, r/w, present bits */
	movl $pg1 + 7, pg_dir + 4								/* user, r/w, present bits */
	movl $7, %eax
	movl $pg0, %edi
	movl $2048, %ecx
1:	stosl
	addl $4096, %eax
	decl %ecx
	jnz 1b

	movl $pg_dir, %eax										/* load pg_dir */
	movl %eax, %cr3
	movl %cr0, %eax
	orl $0x80000000, %eax
	movl %eax, %cr0
	ret

.align 2
ignore_int:
	movb $2, %al
	incb 0xb8000		                                	/* put something on the screen */
	movb %al, 0xb8001		                                /* so that we know something */
	iret

.align 4
.word 0
idt_d:
    .word 256 * 8 - 1
    .long idt

.align 4
.word 0
gdt_d:
	.word 256 * 8 - 1
	.long gdt

idt:
    .fill 256, 8, 0

gdt:
    .quad 0x0000000000000000	                            /* NULL descriptor */
	.quad 0x00c09a00000007ff	                            /* same, as in boot.s */
	.quad 0x00c09200000007ff	                            /* ... */
	.quad 0x0000000000000000	                            /* TEMPORARY - don't use */
	.fill 252, 8, 0			                                /* space for LDT's and TSS's etc */
