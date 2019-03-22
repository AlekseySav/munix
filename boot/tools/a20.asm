;
; a20 utility
; enables A20 line (need for 32-bit system)
;
a20:
    push eax
    cli

    call a20wait
    mov al, 0xad
    out 0x64, al

    call a20wait
    mov al, 0xd0
    out 0x64, al
 
    call a20waitd
    in al, 0x60
    push eax
 
    call a20wait
    mov al, 0xd1
    out 0x64, al
 
    call a20wait
    pop eax
    or al, 2
    out 0x60, al
 
    call a20wait
    mov al, 0xae
    out 0x64, al
 
    call a20wait

    sti
    pop eax
    ret

a20wait:                                    ; wait until release keyboard
    in al, 0x64
    test al, 2
    jnz a20wait
    ret

a20waitd:                                   ; wait until get key data
    in al, 0x64
    test al, 1
    jz a20waitd
    ret
