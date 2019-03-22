;
; print utility
; input: si - msg to print ending on 0
; output: si - end of msg
;
print:
    push ax
    push bx
    mov ah, 0x0e                            ; (used bios interrupt 0x10) ah=0x0e puts symbol on screen
    xor bh, bh                              ; page = 0
    mov bl, 0x07                            ; if graphical mode - white text on black screen
putc:
    lodsb                                   ; msame as, mov al, [si]; si++
    or al, al
    jz print_end                            ; if al is 0 return
    int 0x10                                ; else print it
    jmp putc
print_end:
    pop bx
    pop bx
    ret
