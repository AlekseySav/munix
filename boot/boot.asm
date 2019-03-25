;
; boot.asm
;
; this bootloader is loading by BIOS to 0x07c segment
; then it copies itself to INITSEG (0x9000)
; print message, that is's loaded
; then it loads kernel to SYSSEG and close floppy
; and, after it loads descriptors and switches to protected mode
;
; NOTE! max size of this kernel is ~0x80000 bytes = 512KiB
; There won't be any problems with it now
; But they may appear in the future
;
; NOTE(2)! this bootloader read system from floppy
; So, it must be loaded from floppy (or from disk img in VM)
; To loaded it from other device,
; You must load system in first bytes of its memory
; And comment/delete "call killm" command
;

BOOTSEG         equ 0x07c0                  ; bios load bootloader here
INITSEG         equ 0x9000                  ; it copies itself here
SYSSEG          equ 0x0100                  ; kernel is loaded here
SYSSIZE         equ 0x8000                  ; size of kernel (in 16 * bytes)

entry:                                      ; start of bootloader
    jmp BOOTSEG:start                       ; far jump to bootseg
start:
    mov ax, BOOTSEG
    mov ds, ax                              ; ds = BOOTSEG
    mov ax, INITSEG
    mov es, ax                              ; es = INITSEG
    mov cx, 256                             ; cx = 256 = size of boot in words
    xor di, di
    xor si, si
    cld
    rep                                     ; mov cx(256) words (all bootloader)
    movsw                                   ; from bootseg to initseg
    jmp INITSEG:go                          ; far jump to initseg
go:
    cli
    mov ax, cs                              ; set all segments to current
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x400                           ; stack size is 512 words
    sti
    mov si, msg1
    call print                              ; from tools/print.asm

    mov ax, SYSSEG
    mov es, ax                              ; es = SYSSEG
    mov ax, SYSSIZE
    call readk                              ; read system
    call killm                              ; kill monitor

    mov ax, cs
    mov es, ax                              ; es = cs again

    mov ah, 0x03                            ; read cursor pos (to dx)
    xor bh, bh
    int 0x10
    mov [ds:0x1fe], dx                      ; save it to 0x901fe (there is 0xaa55 now)

    cli                                     ; no interrupts allowed
    call line20

    lidt [idt_d]                            ; load descriptors
    lgdt [gdt_d]

    mov eax, cr0
    or ax, 1
    mov cr0, eax                            ; set pmode bit activated

    jmp 8:SYSSEG * 16

die:
    mov si, err
    call print                              ; from tools/print.asm
die_loop:
    xor ah, ah
    int 0x16                                ; read key
    mov ah, 0x0e
    int 0x10                                ; print pushed symbol
    cmp al, 'y'                             ; if ASCII key is 'y' - reboot
    jne die_loop                            ; if not wait for key again
    jmp INITSEG:go                          ; else jump to start

%include "boot/tools/print.asm"
%include "boot/tools/readk.asm"
%include "boot/tools/line20.asm"

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

idt_d:                                      ; idt descriptor
    dw 0
    dd 0                                    ; idt uninitialized

msg1:
    db 13, 10, 13, 10, "Munix 0.0.1 booting...", 13, 10, 0
err:
    db 13, 10, "E: Failed booting Munix. Reboot? [y/n]", 13, 10, 0

    times 510-($-$$) db 0
    dw 0xaa55
