start:
    mov eax, 0xb8000
    xor cx, cx
    mov gs, cx
    mov bx, ' '
    mov [gs:eax], bx

    jmp $
