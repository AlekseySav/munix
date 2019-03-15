global start
extern main

section .text
start:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    call main
end:
    cli
    hlt
    jmp $

idt:
    resb 256 * 8

idt_d:
    dw 256 * 8 - 1
    dd idt
