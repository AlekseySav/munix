.globl _start

_start:
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    xor %esp, %esp
1:  jmp 1b


/*
this is only part of 
bootloader's head
*/
