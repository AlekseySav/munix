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

puts:
    pusha
    call cursor
    mov ax, 0x1301
    xor bh, bh
    int 0x10
    popa
    ret

putc:
    push ax
    mov ah, 0x0e
    int 0x10
    pop ax
    ret

putl:
    push ax
    mov al, 13
    call putc
    mov al, 10
    call putc
    pop ax
    ret

pdone:
    pusha
    push bp
    mov al, ' '
    call putc
    mov al, '['
    call putc
    mov bp, done
    mov bx, 0x02
    mov cx, 6
    call puts
    mov al, ']'
    call putc
    call putl
    pop bp
    popa
    ret

perr:
    pusha
    push bp
    call putl
    mov bx, 0x04
    mov cx, 1
    mov bp, perr.err
    call puts
    mov bp, err
    mov bx, 0x07
    mov cx, 23
    call puts
    call putl
    pop bp
    popa
    ret

getc:
    push bx
    mov bx, 0x07
    xor ah, ah
    int 0x16
    or al, al
    jz getc.end
    call putc
getc.end:
    pop bx
    ret

readk:
    call putl
    pusha
    push bp
    mov bx, 0x07
    mov cx, 18
    mov bp, msg2
    call puts
    pop bp
    popa

    pusha
    xor ah, ah
    xor dx, dx
    int 0x13
    popa
    jc die

    pusha
    push es
    push di
    mov ax, 0x0800
    mov es, ax
    xor di, di
    mov ah, 0x02
    mov al, 17
    xor bx, bx
    xor ch, ch
    mov cx, 0x02
    xor dx, dx
    int 0x13
    pop di
    pop es
    popa
    jc die

    call pdone
    mov ax, 0x0800
    ret

killm:    
    push dx 
    push ax 
    mov dx, 0x3f2 
    xor al, al 
    out dx, al 
    pop ax 
    pop dx 
    ret

start:
    cli
    mov ax, 0x07c0
    mov ds, ax
    mov ax, 0x0600
    mov es, ax
    mov cx, 0x100
    xor di, di
    xor si, si
    cld
    rep
    movsw
    sti
    jmp 0x0600:go
go:
    cli
    mov ax, cs
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov sp, 0x400
    sti

    call putl
    call putl
    mov bx, 0x07
    mov cx, 37
    mov bp, msg1
    call puts
    call putl
sleep:
    call getc
    cmp ah, 0x1c
    jne sleep

    call readk

    mov bx, 0x07
    mov cx, 19
    mov bp, msg3
    call puts

    call killm
    jc die
    cmp ax, 0x0800
    jne die
    call pdone
    jmp 0x0800:0
die:
    call perr
    hlt
end:
    jmp $

msg1:
    db "MUNIX 0.0.1 (Press Enter to continue)"
msg2:
    db "Loading system ..."
msg3:
    db "Confirm changes ..."
done:
    db " done "
perr.err:
    db "E"
err:
    db ": Failed booting kernel"

times 510 - ($ - $$) db 0
dw 0xaa55
