BOOTSEG         equ 0x07c0
INITSEG         equ 0x9000
SYSSEG          equ 0x0060
SYSSIZE         equ 18

start:
    mov ax, BOOTSEG
    mov ds, ax
    mov ax, INITSEG
    mov es, ax
    mov cx, 0x100
    xor di, di
    xor si, si
    cld
    rep
    movsw
    jmp INITSEG:go
go:
    cli
    mov ax, INITSEG
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x400
    sti

    mov si, msg
    call print

    mov ax, SYSSEG
    call readk
    call killm

    mov ah, 0x03
    xor bh, bh
    int 0x10
    mov ax, 0x08
    mov gs, ax
    mov word [gs:0x9500], dx

    mov ax, 0x2401
    int 0x15

    cli

    mov ax, SYSSEG
    xor bx, bx
move:
    cmp ax, INITSEG
    je move.end
    mov ds, ax
    mov es, bx
    mov cx, 0x100
    xor di, di
    xor si, si
    cld
    rep
    movsw
    inc ax
    inc bx
    jmp move
move.end:
    mov ax, cs
    mov ds, ax
    mov es, ax

    lidt [idt_d]
    lgdt [gdt_d]

    mov ax, 1
    lmsw ax

    jmp 8:0

die:
    mov si, err
    call print
end:
    cli
    hlt
    jmp $

readk:
    cmp ax, SYSSEG
    jne die
    mov es, ax

    push ax
    xor ah, ah
    int 0x13
    pop ax
    jc die

    pusha
    mov ah, 0x02
    mov al, 0x01
    xor bx, bx
    xor dx, dx
    mov cx, 9
    int 0x13
    popa
    jc die
    
    pusha
    mov ah, 0x02
    mov al, 0x01
    xor bx, bx
    xor dx, dx
    mov cx, 15
    int 0x13
    popa
    jc s_9
    
    pusha
    mov ah, 0x02
    mov al, 0x01
    xor bx, bx
    xor dx, dx
    mov cx, 18
    int 0x13
    popa
    jc s_15    
load:
    pusha
    mov ah, 0x02
    mov al, SYSSIZE
    xor bx, bx
    mov cx, 0x02
    xor dx, dx
    int 0x13
    popa
    jc die

    ret

s_9:
    push ax
    mov al, 9
    jmp s_edit
s_15:
    push ax
    mov al, 15
    jmp s_edit
s_edit:
    mov byte [sectors], al
    pop ax
    jmp load

killm:
	push dx
	mov dx, 0x03f2
	xor al, al
	out dx, al
	pop dx
    ret

print:
    pusha
    mov ah, 0x0e
    mov bx, 0x07
print.c:
    lodsb
    or al, al
    jz print.e
    int 0x10
    jmp print.c
print.e:
    popa
    ret

gdt:
    dq 0
    
    dw 0xffff, 0
    db 0
    db 10011010b
    db 11001111b
    db 0
    
    dw 0xffff, 0
    db 0
    db 10010010b
    db 11001111b
    db 0
gdt_end:

gdt_d:
    dw gdt_end - gdt
    dd gdt + INITSEG * 16

idt_d:
    dw 0
    dd 0

sectors:
    db 18
msg:
    db 13, 10, 13, 10, "Munix 0.0.1 booting...", 13, 10, 0
err:
    db 13, 10, "E: Failed booting Munix", 13, 10, 0

    times 510 - ($-$$) db 0
    dw 0xaa55
