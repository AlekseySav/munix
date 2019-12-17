/*
 * asm.S describes low-level traps handles
 */

.globl divide_error, debug_exception, nmi_interrupt, breakpoint
.globl overflow, bound_range_exceeded, invalid_opcode, device_not_available
.globl double_fault, coprocessor_overrun, invalid_tss, segment_not_present
.globl stack_fault, general_protection, page_fault, intel_reserved
.globl math_fault, alignment_check, machine_check, float_exception, virtual_exception

divide_error:
    pushl $0
    jmp no_error_code

debug_exception:
    pushl $1
    jmp no_error_code

nmi_interrupt:
    pushl $2
    jmp no_error_code

breakpoint:
    pushl $3
    jmp no_error_code

overflow:
    pushl $4
    jmp no_error_code

bound_range_exceeded:
    pushl $5
    jmp no_error_code

invalid_opcode:
    pushl $6
    jmp no_error_code

device_not_available:
    pushl $7
    jmp no_error_code

double_fault:
    pushl $8
    jmp error_code

coprocessor_overrun:
    pushl $9
    jmp no_error_code

invalid_tss:
    pushl $10
    jmp error_code

segment_not_present:
    pushl $11
    jmp error_code

stack_fault:
    pushl $12
    jmp error_code

general_protection:
    pushl $13
    jmp error_code

page_fault:
    pushl $14
    jmp error_code

intel_reserved:
    pushl $15
    jmp no_error_code

math_fault:
    pushl $16
    jmp no_error_code

alignment_check:
    pushl $17
    jmp error_code

machine_check:
    pushl $18
    jmp no_error_code

float_exception:
    pushl $19
    jmp no_error_code

virtual_exception:
    pushl $20
    jmp no_error_code

no_error_code:
    xchgl %eax, (%esp)
    pushl %ebx
    movl %eax, %ebx                     # ebx = nr
    xorl %eax, %eax                     # eax = 0 (default error code)
    jmp call_exception
error_code:
    cli
    xchgl %eax, 4(%esp)                 # eax = error code
    xchgl %ebx, (%esp)                  # ebx = nr
call_exception:
    pushl %ecx
    pushl %edx
    pushl %edi
    pushl %esi
    pushl %ebp
    pushw %ds
    pushw %es
    pushw %fs
    pushw %gs
    movw $0x10, %dx
    movw %dx, %ds
    movw %dx, %es
    movw %dx, %fs
    movw %dx, %gs
    leal 36(%esp), %edx
    pushl %edx
    pushl %eax                          # eax = error code
    pushl %ebx                          # ebx = exception nr
    calll exception
    addl $12, %esp
    popw %gs
    popw %fs
    popw %es
    popw %ds
    popl %ebp
    popl %esi
    popl %edi
    popl %edx
    popl %ecx
    popl %ebx
    popl %eax
    sti
    iret
