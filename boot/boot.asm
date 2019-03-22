BOOTSEG     equ 0x07c0                      ; BIOS load boot here
INITSEG     equ 0x9000                      ; bootloader copy itself here
SYSSEG      equ 0x0060                      ; first free memory; we load kernel here
SYSSIZE     equ 0x0001                      ; size of kernel (in sectors)

entry:                                      ; program starts here
    jmp BOOTSEG:start                       ; long jump to bios segment
start:
    mov ax, BOOTSEG
    mov ds, ax
    mov ax, INITSEG
    mov es, ax
    mov cx, 0x100
    xor di, di
    xor si, si
    cld
    rep                                     ; repeat movsw cx (256) times
    movsw                                   ; same as, mov [es:di], [ds:si]; di++; si++
    jmp INITSEG:go                          ; jump to our new segment offset go
go:                                         ; we are in initseg
    cli
    mov ax, cs                              ; load segments now
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x400                           ; stack size will be 0x200 (512) words
    sti

    mov si, msg
    call print                              ; print msg if all right

    mov ah, 0x03                            ; (used bios interrupt 0x10) ah=0x03 get cursor
    xor bh, bh
    int 0x10                                ; read cursor pos to dx

    mov [0x1fe], dx                         ; save it to use later (it's on 0x90510)

    mov ax, SYSSEG
    call readk                              ; read kernel now
    call killm                              ; close floppy

    call move
    call a20

    cli    
    lidt [idt_d]                            ; clear idt
    lgdt [gdt_d]                            ; load gdt

    mov ax, 1
    lmsw ax

    jmp 8:0                                 ; far jump to system
    
die:                                        ; jump here if something was wrong
    mov si, err
    call print                              ; print error
    cli
    hlt
    dw 0xfeeb                               ; loop forever

%include "boot/tools/readk.asm"
%include "boot/tools/print.asm"
%include "boot/tools/move.asm"
%include "boot/tools/a20.asm"

gdt:
    dq 0                                    ; null descr
    
    dw 0xffff, 0                            ; code descr (0x08 seg)
    db 0
    db 10011010b
    db 11001111b
    db 0
    
    dw 0xffff, 0                            ; data descr (0x10 seg)
    db 0
    db 10010010b
    db 11001111b
    db 0

gdt_d:                                      ; gdt descriptor
    dw 3 * 8 - 1
    dd gdt + INITSEG * 16

idt_d:                                      ; null idt descriptor
    dw 0
    dd 0

msg:
    db 13, 10, 13, 10, "Munix 0.0.1 booting...", 13, 10, 0
err:
    db 13, 10, "E: Failed booting Munix", 13, 10, 0

    resb 510 - ($-$$)                       ; make sure, size of our boot is 512 (one segment) 
    dw 0xaa55                               ; magic number
