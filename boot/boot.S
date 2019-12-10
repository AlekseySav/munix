.code16
.globl start

#include <munix/config.h>

/*
 * boot/boot.S
 *
 * boot.S is low 16-bit startup of munix. It is loaded to memory by bios.
 * boot.S loads SYSSIZE sectors of kernel, turns on protected 32-bit mode
 * and jumps to entry point of munix (boot/head.S). SYSSIZE is defined in munix/config.h
 *
 * SMALL NOTE: max size of kernel is 0x90000-0x10000 = 512KiB = 1024 sectors.
 * Hopefuly, for current version of munix it is more than enough.
 * NOTE2: boot.S doesn't take care about partition tables, etc. 
 * It loads data from first sectors of drive/floppy, so on that drive can't be installed anything, except of munix.
 */

BOOTSEG     = 0x07c0
SYSSEG      = 0x1000

.text

start:
    movw $BOOTSEG, %ax
    movw %ax, %ds
    movw $INITSEG, %ax
    movw %ax, %es
    movw $256, %cx
    xorw %di, %di
    xorw %si, %si
    cld
rep movsw                   /* copy bootloader to initseg */
    ljmp $INITSEG, $go
go:
    movw %cs, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs

    cli
    movw %ax, %ss
    movw $0x400, %sp        /* stack space in 512 bytes after boot */
    sti

    call print
.ascii "\n\rLoading system ...\n\r\n\r\0"

    movw $SYSSEG, %ax
    movw %ax, %es
    movw $SYSSIZE, %ax
    call read_kernel        /* read rest part of kernel from drive */
    call kill_motor         /* close floppy, if booted from it */
    
    movw %cs, %ax
    movw %ax, %ds
    movw %ax, %es
    movb $0x03, %ah
    xorw %bx, %bx
    int $0x10               /* get cursor pos */
    movw %dx, 0             /* save to INITSEG:0 */

    call enable_a20         /* finally, enable a20 line before disabling bios */

    cli

    xorw %ax, %ax
    movw $SYSSIZE, %dx
move:                       /* move kernel to 0:0 */
    movw %ax, %es
    addw $SYSSEG, %ax
    movw %ax, %ds
    xorw %di, %di
    xorw %si, %si
    movw $256, %cx
rep movsw
    subw $SYSSEG - 0x20, %ax
    decw %dx
    jnz move

    movw %cs, %ax
    movw %ax, %ds
    movw %ax, %es

    lidt idt_d
    lgdt gdt_d

    movw $0x0001, %ax
    lmsw %ax                /* set PE byte */

    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs           /* setup segments */

    ljmp $0x08, $0          /* jump to 32 bit kernel */

die:
    call print
.ascii "Failed booting Munix\n\r\0"
1:  jmp 1b


/*
 * read_kernel
 * this is the main utility in boot.S
 * input: 
 *  es = first segment of output buffer
 *  ax = nr of sectors to read (can't exceed 1024)
 *  dl = curret drive
 */
read_kernel:
    testw $0xf800, %ax
    jb die
    movw %ax, size
    movb %dl, dev

    xorb %ah, %ah
    int $0x13           /* reload disk */
    jc die

    pushw %es
    xorw %bx, %bx
    movw %bx, %es
    movb $0x08, %ah     /* get drive data */
    int $0x13
    jc die
    popw %es

    incb %dh
    movb %dh, heads
    andb $0x3f, %cl
    movb %cl, sectors
rp_read:                /* rp_read reads one sector */
    xorw %bx, %bx
    xorw %dx, %dx
    movw curr, %ax
    movb sectors, %bl
    divw %bx
    incw %dx
    movw %dx, %cx

    xorw %bx, %bx
    xorw %dx, %dx
    movb heads, %bl
    divw %bx            /* ax = cylinder, dx = head */
    
    movb %al, %ch
    shrw $2, %ax
    andw $0xc0, %ax
    orb %al, %cl
    shlw $8, %dx

    xorw %bx, %bx
    movw $0x0201, %ax   /* read one sector */
    movb dev, %dl

    int $0x13
    jc die

    movw curr, %ax
    cmp size, %ax
    je read_ok
    incw curr
    mov %es, %ax
    add $0x20, %ax      /* next sector */
    mov %ax, %es
    jmp rp_read
read_ok:
    ret

/* varibles, used in read_kernel */
size:       .short  0
curr:       .short  1
dev:        .byte   0
heads:      .byte   0
sectors:    .byte   0


kill_motor:
    movb dev, %dl
    testb $0x80, %dl
    jnz not_fd
	movw $0x03f2, %dx
	movb $0x02, %al
    outb %al, %dx
not_fd:
    ret


enable_a20:
    call a20_wait1
    movb $0xad, %al
    outb %al, $0x64             /* disable first PS/2 port */

    call a20_wait1
    movb $0xd0, %al
    outb %al, $0x64             /* read data */

    call a20_wait2
    inb $0x60, %al
    movb %al, %cl               /* save data value */

    testb $0x02, %cl
    jnz a20_enabled
    orb $0x02, %cl

    call a20_wait1
    movb $0xd1, %al
    outb %al, $0x64             /* write data */

    call a20_wait1
    movb %cl, %al
    outb %al, $0x60             /* set a20 bit */
a20_enabled:
    call a20_wait1
    movb $0xae, %al
    outb %al, $0x64             /* enable first PS/2 port */

    ret

/* 
 * empty_8042 used in enable_a20.
 * it waits until 8042 PS/2 become empty
 */
a20_wait1:
    movw $0x64, %dx
    inb %dx, %al
    testb $0xc0, %al
    jnz die                     /* some errors occurred */
    testb $0x02, %al
    jnz a20_wait1               /* input not empty */
    ret

a20_wait2:
    movw $0x64, %dx
    inb %dx, %al
    testb $0xc0, %al
    jnz die                     /* some errors occurred */
    testb $0x01, %al
    jz a20_wait2                /* output empty */
    ret

/*
empty_8042:
    movw $0x64, %dx
    inb %dx, %al
    testb $0xc0, %al
    jnz die
    testb %bl, %al
    jnz empty_8042
    ret
*/


print:
    popw %si
    movw $0x0e00, %ax
    movw $0x0001, %bx
pr_loop:
    lodsb
    orb %al, %al
    jz pr_done
    int $0x10
    jmp pr_loop
pr_done:
    jmp * %si


.align 4

/* descriptor tables */
.short 0
idt_d:
    .short 0, 0, 0

.short 0
gdt_d:
    .short 3 * 8 - 1
    .long INITSEG * 0x10 + gdt

gdt:
    .quad 0                         /* NULL */
    
    .short 0x07ff                   /* 8MiB */
    .short 0x0000
    .short 0x9a00                   /* code */
    .short 0x00c0

    .short 0x07ff
    .short 0x0000
    .short 0x9200                   /* data */
    .short 0x00c0
