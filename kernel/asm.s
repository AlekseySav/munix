.globl time_intr

time_intr:
    movw 0xb8002, %ax
    incb %al
    movw %ax, 0xb8002

    movb $0x20, %al
    outb %al, $0x20
    iret
