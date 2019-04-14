#include <ansi.h>       // must be included; for PACKED
#include <sys/const.h>
#include <asm/system.h> // for sti(), nop()
#include <string.h>

EXTERN void con_init(void);
EXTERN void async_write( const char * ptr);

void main(void)
{
    con_init();
    sti();
    
    async_write("System loaded\n");

    char buf[50];
    int n = 0;
    sprintf(buf, "str\n%3c\n%s\n%03o\n%#p\n%x\n%X\n%d\n%+i\n% u\n", 
        'c', "str", 8, buf, 15, 15, -10, 10, 10);

    async_write((const char *)buf);

    while(TRUE) nop();
}

long stack[1024];

// used in boot/head.s
struct {
    long * ptr;
    short segment;
} PACKED stack_start = { &stack[1024], 0x10 };
