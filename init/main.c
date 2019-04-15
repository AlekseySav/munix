#include <ansi.h>       // must be included; for PACKED
#include <sys/const.h>
#include <asm/system.h> // for sti(), nop()
#include <string.h>
#include <munix/tty.h>

struct tty_struct tty;

void main(void)
{
    con_init();
    sti();
    
    sprintf(tty.write_q.buf, "System loaded\n");
    tty.write_q.tail = 0;
    tty.write_q.head = 0;
    while(tty.write_q.buf[tty.write_q.head++]);

    con_write(&tty);

    while(TRUE);
}

long stack[1024];

// used in boot/head.s
struct {
    long * ptr;
    short segment;
} PACKED stack_start = { &stack[1024], 0x10 };
