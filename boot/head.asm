;
; head.asm 
;
; needs to check and correct
; segments, A20 line, stack, etc.
; and call main (from C-code) next
;
global start                                ; entry symbol
global ignore_int, idt, idt_d, keyh         ; uses for interrupts

extern main, key_handler

start:
    mov ax, 0x10                            ; set segments
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov ebp, 0x90000                        ; set stack
    mov esp, ebp

    push 1                                  ; check stack
    pop ax
    cmp ax, 1                               ; if we pop not 1 that we push - stack crashed
    jne die

    xor eax, eax                            ; check A20 line
    mov eax, 0x10000
    cmp eax, 0x10000                        ; if eax is not 0x10000 - A20 line crashed
    jne die

    jmp init
    times 0x200 - ($-$$) db 0               ; reserve free space in the begin of memory (0x00eb = jmp $+2)
init:
    call main                               ; now we load kernel main function
die:
    cli
    hlt
    jmp $

ignore_int:                                 ; default interrupt handler
    iret

keyh:                                       ; keyboard interrupt handler
    in al, 0x60                             ; get key data
    mov bl, al                              ; save it
    xor edx, edx
    mov byte dl, al

    in al, 0x61                             ; keybrd control
    mov ah, al
    or al, 0x80                             ; disable bit 7
    out 0x61, al                            ; send it back
    xchg ah, al                             ; get original
    out 0x61, al                            ; send that back

    mov al, 0x20                            ; End of Interrupt
    out 0x20, al

    and bl, 0x80                            ; key released
    jnz done                                ; don't repeat
done:
    push edx
    call key_handler
    pop edx
    iret

idt:
    times 256 * 8 db 0                      ; interrupt table: 256 entries

idt_d:
    dw 256 * 8 - 1
    dd idt
