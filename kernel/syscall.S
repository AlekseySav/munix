.globl system_call

.text

system_call:
    cli
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs
	pushl %edi
	pushl %esi
	pushl %ebp
    pushl %edx      # eax, ebx, ecx are arguments
    pushl %ecx
    pushl %ebx
    movw $0x10, %edx
    movw %dx, %ds
    movw %dx, %es
    call syscall_table(, %eax, 4)
    popl %ebx
    popl %ecx
    popl %edx
    popl %ebp
    popl %esi
    popl %edi
    popl %gs
    popl %fs
    popl %es
    popl %ds
    sti
    iret
