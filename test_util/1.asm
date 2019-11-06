bits 32

    xor eax, eax
    int 0x80            ; run check_syscalls()
    ret
