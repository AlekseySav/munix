.globl time_intr

.text

time_intr:
	incb 0xb8000
    
    cli
    pushl %ds
    pushl %es
    pushl %edx
    pushl %ecx
    pushl %ebx
    pushl %eax
    movl $0x10, %edx
    movw %dx, %ds
    movw %dx, %es

    movb $0x20, %al
    outb %al, $0x20
    call schedule

    popl %eax
    popl %ebx
    popl %ecx
    popl %edx
    popl %es
    popl %ds
    sti
    iret
