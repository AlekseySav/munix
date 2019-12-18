.globl timer_intr

.text

timer_intr:
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
    iret
