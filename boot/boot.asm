BOOTSEG     equ 0x07c0
INITSEG     equ 0x0900
SYSSEG      equ 0x0060
MAGIC       equ 0x01fe
SECTORS     equ 15

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
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov sp, 0x400
    sti

    mov si, msg1
    call print

    mov ax, SYSSEG
    call readk

    mov ax, 0x2401
    int 0x15
    
    cli
    lgdt [ds:gdt_d]

    xor ax, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov eax, cr0
    or ax, 1
    mov cr0, eax

    jmp CODESEG:0 + SYSSEG * 16

readk:
    cmp ax, SYSSEG
    jne die
    mov es, ax

    xor ah, ah
    int 0x13
    jc die
    
    mov ah, 0x02
    mov al, 0x01
    xor bx, bx
    mov cx, 0x0f
    xor dx, dx
    int 0x13
    jc die

    mov ah, 0x02
    mov al, SECTORS
    xor bx, bx
    mov cx, 0x02
    xor dx, dx
    int 0x13
    jc die

    ret

print:
    pusha
    push si
    mov ah, 0x0e
    xor bh, bh
    mov bl, 0x07
print.c:
    lodsb
    or al, al
    jz print.e
    int 0x10
    jmp print.c
print.e:
    pop si
    popa
    ret

die:
    mov si, err
    call print
    cli
    hlt
    jmp $

gdt:
    dd 0, 0
gdt_code:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10011010b
    db 11001111b
    db 0x0
gdt_data:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0
gdt_end:


gdt_d:
    dw gdt_end - gdt
    dd gdt + INITSEG * 16

CODESEG equ gdt_code - gdt
DATASEG equ gdt_data - gdt

bits 32
boot2:
    mov ax, DATASEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esi, msg2 + INITSEG * 16
    mov ebx, 0xb8000
.loop:
    lodsb
    or al,al
    jz halt
    or eax,0x0100
    mov word [ebx], ax
    add ebx,2
    jmp .loop
halt:
    cli
    hlt
    jmp $

msg1:
    db 13, 10, 13, 10, "Munix 0.0.1 booting...", 13, 10, 0
msg2:
    db "Loading system...", 0
err:
    db 13, 10, "E: Failed booting Munix", 13, 10, 0

    resb MAGIC - ($-$$)
    dw 0xaa55
