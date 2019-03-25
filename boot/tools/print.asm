;
; print utility
;
; prints msg on the screen, using BIOS interrupts
; input: si - current message to print (must ends on 0)
; output: si - end of msg (so it point to zero)
;

print:
    push ax
    push bx
    mov ah, 0x0e                            ; tell bios to print char
    mov bx, 0x07                            ; write on black (if it's graphical mode)
putc:
    lodsb                                   ; al = byte [si]; si++
    or al, al
    jz print_end                            ; it's end of msg if al is zero
    int 0x10                                ; graphical interrupt (if ah=0x0e - print al as char)
    jmp putc
print_end:
    dec si                                  ; si must point to zero
    pop bx
    pop ax
    ret


;
; TOOLS FOR DEBUG
;
; NOTE! this commands are used for debug only
; delete/comment them if you don't want to use it for debugging
;

;
; print reg utility
; prints ax on screen as a number
;

; print_reg:
;     pusha
;     mov di, pr_out
;     mov si, pr_use
;     mov cx, 4                               ; four places
; putr:
;     rol ax, 4                               ; leftmost will
;     mov bx, ax                              ; become
;     and bx, 0x0f                            ; rightmost
;     mov bl, [si + bx]                       ; index into hexstr
;     mov [di], bl
;     inc di
;     dec cx
;     jnz putr

;     mov si, pr_str
;     call print
;     popa
;     ret

; pr_str:
;     db "0x"
; pr_out:
;     db "0000", 13, 10, 0
; pr_use: 
;     db "0123456789abcdef"
