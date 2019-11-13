.globl divide_error, debug_except, nomask_intr
.globl breakpoint, overflow, bounds_check
.globl invalid_opcode, coprocessor_na, double_fault
.globl coprocessor_overrun, invalid_tss, segment_np
.globl stack_exept, general_protect, page_fault
.globl reserved_trap, coprocessor_error

.text

divide_error:
    pushl $0
    jmp _trap_die

debug_except:
    pushl $1
    jmp _trap_die

nomask_intr:
    pushl $2
    jmp _trap_die

breakpoint:
    pushl $3
    jmp _trap_die

overflow:
    pushl $4
    jmp _trap_die

bounds_check:
    pushl $5
    jmp _trap_die

invalid_opcode:
    pushl $6
    jmp _trap_die

coprocessor_na:
    pushl $7
    jmp _trap_die

double_fault:
    pushl $8
    jmp _trap_die

coprocessor_overrun:
    pushl $9
    jmp _trap_die
    
invalid_tss:
    pushl $10
    jmp _trap_die

segment_np:
    pushl $11
    jmp _trap_die

stack_exept:
    pushl $12
    jmp _trap_die

general_protect:
    pushl $13
    jmp _trap_die
    
page_fault:
    pushl $14
    jmp _trap_die

reserved_trap:
    pushl $15
    jmp _trap_die

coprocessor_error:
    pushl $16
    jmp _trap_die

_trap_die:
    cli
    call exception
    sti
    iret                # as usual, program won't return here
