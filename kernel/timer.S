.globl timer_intr

timer_intr:
    incb 0xb8000

    cli
    pushal
    pushw %ds
    pushw %es
    pushw %fs
    pushw %gs

    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs

    movb $0x20, %al
    outb %al, $0x20             # EOI
    call shedule

    popw %gs
    popw %fs
    popw %es
    popw %ds
    popal
    sti
    iret
