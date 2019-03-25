;
; head.asm 
;
; needs to check and correct
; segments, A20 line, stack, etc.
; and call main (from C-code) next
;
global start                                ; entry symbol
global ignore_int, idt, idt_d               ; uses for interrupts

extern main

start:
    mov ax, 0x10                            ; set segments
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov ebp, 0x90000                        ; set stack
    mov esp, ebp

    push 1                                  ; check stack
    pop ax
    cmp ax, 1                               ; if we pop not 1 that we push - stack crashed
    jne die

    xor eax, eax                            ; check A20 line
    mov eax, 0x10000
    cmp eax, 0x10000                        ; if eax is not 0x10000 - A20 line crashed
    jne die

    jmp init
    times 0x200 - ($-$$) db 0               ; reserve free space in the begin of memory (0x00eb = jmp $+2)
init:
    call main                               ; now we load kernel main function
die:
    cli
    hlt
    jmp $

ignore_int:                                 ; default interrupt handler
    mov bx, 0x0430
    mov [0xb8000], bx                       ; put red 0 in the start of screen
    iret

idt:
    times 256 * 8 db 0                      ; interrupt table: 256 entries

idt_d:
    dw 256 * 8 - 1
    dd idt
