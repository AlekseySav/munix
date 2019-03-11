global start
extern main

start:
    cli
    call main
    hlt
    jmp $
