.globl sys_call

sys_call:
    pushl %ebx
    pushl %ecx
    pushl %edx
    call sys_call_table(%eax)
    popl %edx
    popl %ecx
    popl %ebx
    iret
