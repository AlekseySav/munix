;
; test kernel...
;
section .text
global start

start:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ah, 0x02
    mov ebx, 0xb8000
    mov si, msg
print:
    lodsb
    or al, al
    jz end
    mov [ebx], ax
    add bx, 2
    jmp print
end:
    cli
    hlt
    jmp $
    ret

msg: 
    db "Hello, world!", 0
