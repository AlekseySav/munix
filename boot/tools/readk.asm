;
; readk
;
; read system from floppy
; input:
; es - segment to save system
; ax - size of system (in bytes)
; NOTE! ax must be devided by 32 (or last sector wouldn't be loaded)
; output:
; es - end of system
;

readk:
    pusha
    shr ax, 5                               ; from bytes to sectors (equal ax = ax / 32)
    mov [size], ax

    xor ah, ah                              ; tell bios to reload disk
    xor dl, dl                              ; dl = 0 - use curr disk
    int 0x13                                ; bios interrupt working with disks
    jc die                                  ; error? jump to die

    mov ah, 0x08                            ; learn disk geometry
    xor dl, dl                              ; our drive
    push es
    xor di, di                              ; need to correct work (some bioses crash if not)
    mov es, di
    int 0x13
    pop es
    jc die

    inc dh                                  ; dh + 1 is number of heads per track
    and cl, 00111111b                       ; 6 lowest bits of cl is secctors per track
    mov [heads], dh                         ; save heads/track
    mov [sectors], cl                       ; save sectors/track
rp_read:
    call read_setreg                        ; set correct registers for 0x13
    int 0x13
    jc die

    mov ax, [curr]
    inc ax
    mov [curr], ax

    mov ax, es
    add ax, 32                              ; 32 = 512 / 16 (go to next segment)
    mov es, ax

    mov ax, [curr]
    sub ax, 2
    cmp ax, [size]                          ; have we read all yet?
    jnl read_end

    jmp rp_read
read_end:
    popa
    ret

read_setreg:
    mov ax, [curr]
    dec ax
    mov bl, [sectors]
    div bl                                  ; ah = cuur % sectors; al = curr / sectors
    mov bl, al
    inc ah                                  ; ah is current sector now
    mov dl, ah                              ; save it
    xor bh, bh
    mov ax, bx                              ; ax = bl = curr / sectors
    mov bl, [heads]
    div bl                                  ; ah = curr head, al = curr cylinder

    mov ch, al
    and ch, 0xff                            ; ch = cylinder & 0xff
    mov cl, al
    shr cl, 2
    and cl, 0xc0
    or cl, dl                               ; dl is stil current sector
    mov dh, ah                              ; dh is current head now

    mov ah, 0x02                            ; tell bios to read sectors
    mov al, 0x01                            ; 1 sector
    xor bx, bx                              ; es:bx - output buffer
    xor dl, dl                              ; our drive
    ret

heads:
    db 0
sectors:
    db 0
size:
    dw 0
curr:
    dw 2

;
; killm
; close floppy
;

killm:
    push ax
	push dx
	mov dx, 0x3f2
	xor al, al
	out dx, ax
	pop dx
    pop ax
	ret
