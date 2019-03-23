global start, ignore_int
extern main

section .text

start:
    mov ax, 0x10                            ; set segments
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    mov esp, stack                          ; set stack
    push 0xffff                             ; check stack
    pop ax
    cmp ax, 0xffff
    jne err
    
    xor eax, eax                            ; check A20 line
    inc eax
    cmp eax, 1
    jne err

    mov eax, cr0                            ; set cr0 chip
    and eax, 0x80000011
    or eax, 0x0100
    mov cr0, eax
    jc err

    call main                               ; launch C :-)
err:
    cli
    hlt
    dw 0xfeeb


ignore_int:                                 ; uses to set IDT
    mov ax, 0x7020                          ; fill white left-top part of screen
    mov word [0xb8000], ax
    iret

section .bss
    resw 2048                               ; stack size is 2048 words
    stack:
