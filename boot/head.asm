[bits 16]
global start

start:
    mov dx, word [0x510]

    mov ax, 0x0060
    mov ds, ax
    mov es, ax

    mov word [pos], dx
    call cursor      ; init cursor
    cld
    sti

    mov al, 'F'
    mov ah, 0x07
    call putc
    
    jmp end
end:
    jmp $

;
; putc function
; input:
; ax - char and flags
;
putc:
    push eax
    pusha
    push gs
    mov bx, ax
    mov dx, word [pos]
    xor eax, eax
    mov cl, 80
    add al, dh
    mul cl
    add al, dl
    shl ax, 1
    add eax, 0xb8000
    xor cx, cx
    mov gs, cx
    mov word [gs:eax], bx
    inc dl
    cmp dl, 80
    jl putc.e
    xor dl, dl
    inc dh
putc.e:
    mov word [pos], dx
    call cursor
    pop gs
    popa
    pop eax
    ret

;
; like print function
; from boot.asm
; output: si - end of str
; al - 0 if all was sucsessful
;
puts:
    lodsb
    ;or al, al
    ;jnz print.e
    call putc
    ;jmp puts
print.e:
    ret

;
; change cursor pos to (x, y)
;
cursor:
    pusha
    cli
    mov al, [y]
    mov cl, 80
    mul cl
    add ax, [x]
    mov bx, ax
    mov cx, ax
    and cx, 0xff
    shr bx, 8
    and bx, 0xff

    mov dx, 0x3d4
    mov al, 14
    out dx, al
    mov dx, 0x3d5
    mov al, bl
    shr al, 2
    out dx, al
    mov dx, 0x3d4
    mov al, 15
    out dx, al
    mov dx, 0x3d5
    mov al, cl
    out dx, al
    sti
    popa
    ret

pos:
x:  db 0
y:  db 0
