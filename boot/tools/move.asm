;
; move utility
; movs system from sysseg to initseg
;
move:
    pusha
    push ds
    push es
    cli

    mov ax, SYSSEG
    mov bx, 0                               ; now we want to move system to 0x000
    mov cx, 0x100
do_move:
    mov ds, ax
    mov es, bx
    xor di, di
    xor si, si
    rep
    movsw                                   ; mov one segment
    inc ax
    inc bx
    cmp ax, INITSEG                         ; if ax >= initseg return (end of system)
    jne do_move                             ; we only incremented ax, so wu could check only equ

    pop es
    pop ds
    popa
    sti
    ret
