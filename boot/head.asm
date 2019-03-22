global start
extern main

section .text

start:
    mov ax, 0x10                            ; set segments
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    call main                               ; launch C :-)
    cli
    hlt
    jmp $
