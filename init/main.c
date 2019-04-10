#include <ansi.h>
#include <sys/const.h>
#include <asm/system.h>

extern void con_init(void);
extern void async_write(const char * ptr);

void main(void)
{
    con_init();
    sti();
    
    async_write("Munix loaded\n");
    while(1);
}

long stack[1024];

struct {
    long * ptr;
    short segment;
} PACKED stack_start = { &stack[1024], 0x10 };
