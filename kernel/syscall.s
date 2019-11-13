.globl sys_call

sys_call:
    pushl %ds
    pushl %es
    pushl %edx
    pushl %ecx
    pushl %ebx
    movl $0x10, %edx
    movw %dx, %ds
    movw %dx, %es
    call sys_call_table(, %eax, 4)
    popl %ebx
    popl %ecx
    popl %edx
    popl %es
    popl %ds
    iret
