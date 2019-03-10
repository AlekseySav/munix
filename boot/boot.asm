BOOTSEG     equ 0x07c0 ; BIOS loads boot here
INITSEG     equ 0x9000
SYSSEG      equ 0x0060 ; First free memory
MAGIC       equ 0x01fe ; MAGIC (0xAA55) location
SECTORS     equ 15     ; sizeof kernel
TRACKSIZE   equ 15     ; can't be more than 15

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
    mov ax, cs
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

    mov si, msg2
    call print

    mov dx, 0x03f2
    mov al, 0x0c
    out dx, al      ; kill monitor

    mov ah, 0x03
    xor bh, bh
    int 0x10
    mov word [0x510], dx

    cli

    jmp SYSSEG:0
    jmp die


;
; read kernel function
; input: ax - kernel new place
; ax must be #SYSSEG
;
readk:
    cmp ax, SYSSEG
    jne die
    
    push ax
    xor ax, ax
    int 0x13    ; reload disk
    pop ax
    jc die

    mov es, ax
    xor bx, bx
    mov ah, 0x02
    mov al, 0x01
    mov cx, TRACKSIZE
    xor dx, dx
    int 0x13    ; check, that there're TRACKSIZE sectors/track
    jc die
    
    mov ah, 0x02
    mov al, SECTORS
    mov cx, 0x02
    xor dx, dx
    xor bx, bx
    int 0x13
    jc die
    ret

;
; die partition
; jmp here, if booting failed
;
die:
    mov si, err
    call print
    cli
    hlt
end:
    jmp $
    
;
; print function
; input: si - msg to print
; must have zero at the end
; output: si - end of msg
;
print:
    push ax
    push bx
    mov ah, 0x0e
    xor bh, bh
    mov bl, 0x07
print.n:
    lodsb
    or al, al
    jz print.e
    int 0x10
    jmp print.n
print.e:
    pop bx
    pop ax
    ret

msg1:
    db 13, 10, 13, 10, "Munix 0.0.1 loading...", 13, 10, 0
msg2:
    db "Booting system...", 0
err:
    db 13, 10, "E: Failed booting Munix", 13, 10, 0

    times MAGIC - ($ - $$) db 0
    dw 0xaa55
