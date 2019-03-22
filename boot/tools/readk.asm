;
; readk utility read kernel and load it in ax segment
; ax must be equal SYSSEG
;
sectors:                                    ; sectors per track
    dw 0

sectors_8:                                  ; 8 sectors?
    mov ax, 8
    mov word [sectors], ax
    jmp load

sectors_15:                                 ; 15 sectors?
    mov ax, 15
    mov word [sectors], ax
    jmp load

sectors_18:                                 ; 18 sectors?
    mov ax, 18
    mov word [sectors], ax
    jmp load

readk:
    cmp ax, SYSSEG
    jne die                                 ; exit if ax != SYSSEG

    push ax
    push dx
    xor ax, ax                              ; ax = 0 (if int 0x13) reloading disk
    xor dx, dx                              ; dx = 0 load curr disk
    int 0x13                                ; refresh disk
    pop dx
    pop ax
    jc die                                  ; die if error

    push ax

    push es
    pusha
    mov es, ax
    mov ah, 0x02                            ; ah=0x02 (int 0x13) read sector
    mov al, 0x01                            ; read 1 sector
    xor bx, bx                              ; we load sector by [es:bx]
    mov cx, 15                              ; read 15-en sector
    xor dx, dx
    int 0x13
    popa
    pop es
    jc sectors_8                            ; if error, disk have 8 sectors/track

    push es
    pusha
    mov es, ax
    mov ah, 0x02
    mov al, 0x01
    xor bx, bx
    mov cx, 18                              ; read 18-en sector
    xor dx, dx
    int 0x13
    popa
    pop es
    jc sectors_15                           ; if error, disk have 15 sectors/track
    jnc sectors_18                          ; else we have 18 sectors/track

load:
    mov ax, [sectors]
    or ax, ax                               ; if sectors is still 0 - exit
    pop ax
    jz die

    push ax
    mov ax, SYSSIZE
    cmp ax, [sectors]
    pop ax
    jnl die                                 ; if size of kernel more than our track, exit

    push es
    pusha
    mov es, ax
    mov ah, 0x02
    mov al, SYSSIZE
    xor bx, bx
    mov cx, 2                               ; read sectors since next
    xor dx, dx
    int 0x13                                ; now read kernel
    popa
    pop es
    jc die                                  ; exit if error reading kernel

    ret

;
; killm function
; closes floppy
;
killm:
    push dx 
    push ax 
    mov dx, 0x3f2 
    xor al, al 
    out dx, al 
    pop ax 
    pop dx 
    ret
