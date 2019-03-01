    jmp start

cursor:
    push ax
    push bx
    push cx
    mov ah, 0x03
    xor bh, bh
    int 0x10
    pop cx
    pop bx
    pop ax
    ret

write:
    pusha
    push cx
    call cursor
    pop cx
    mov ax, 0x1301
    int 0x10
    popa
    ret

perr:
    mov bx, 0x04
    call write
    jmp die
    ret

pdone:
    mov bx, 0x07
    mov bp, done
    mov cx, 3
    call write
    mov bx, 0x02
    mov bp, done
    add bp, 3
    mov cx, 4
    call write
    mov bx, 0x07
    mov bp, done
    add bp, 7
    mov cx, 2
    call write
    ret

readk:
    mov bx, 0x07
    mov bp, msg1
    mov cx, 20
    call write

    test ax, 0x0fff
    jne readk_die
readk_load:
    mov bx, ax
    xor ah, ah
    xor dl, dl
    int 0x13
    jc readk_load
    mov ax, bx

    push ds
    push es
    mov es, ax
    mov di, ax
    mov ah, 0x2
    mov al, 0x10
    xor bx, bx
    xor ch, ch
    mov cl, 0x02
    xor dx, dx
    int 0x13
    jc readk_die
    pop es
    pop ds
    
    call pdone
    ret
readk_die:
    mov bp, msg2
    mov cx, 26
    call perr

killm:
    mov bx, 0x07
    mov cx, 21
    mov bp, msg3
    call write
    push dx 
    push ax 
    mov dx, 0x3f2 
    xor al, al 
    out dx, al 
    pop ax 
    pop dx 

    mov al, 0xd1
    out 0x64, al
    mov al, 0xdf
    out 0x60, al
    call pdone
    ret

pmode:
    mov bx, 0x07
    mov cx, 26
    mov bp, msg4
    call write

    ret

start:
    mov ax, 0x07c0
    mov ds, ax
    mov ax, 0x9000
    mov es, ax
    mov cx, 0x100
    xor di, di
    xor si, si
    cld
    rep
    movsw
    jmp 0x9000:go
go:
    mov ax, cs
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov sp, 0x400

    mov bx, 0x07
    mov cx, 15
    mov bp, munix
    call write

    mov ax, 0x1000
    call readk
    call killm

    call cursor
    mov [0x8000], dx
    call pmode

    mov bp, msg2
    mov cx, 26
    call perr

    jmp end
die:
    hlt
end:
    jmp $

munix:
    db 13, 10, "MUNIX 0.0.1", 13, 10
msg1:
    db 13, 10, "Loading system ..."
msg2:
    db 13, 10, 13, 10, "Error loading kernel", 13, 10
msg3:
    db 13, 10, "Confirm changes ..."
msg4:
    db 13, 10, 13, 10, "Switching to pmode ..."
done:
    db " [ done ]"

gdt_d:
    dw gdtend - gdt - 1
    dw gdt, 0x9

gdt: 
    dd 0, 0

flatcode:
    db 0xff, 0xff, 0, 0, 0, 10011010b, 10001111b, 0
flatdata:
    db 0xff, 0xff, 0, 0, 0, 10010010b, 11001111b, 0
gdtend:

times 510 - ($-$$) db 0
dw 0xaa55
