#include <ansi.h>

char value = 'v';
unsigned pos, x, y;

void main(void)
{
    pos = *(unsigned *)(0x90000 + 510);
    x = pos & 0xff;
    y = (pos & 0xff00) >> 8;
    pos = x * 2 + y * 160 + 0xb8000;

    *(char *)pos = value;
    *(char *)(pos + 1) = 0x07;
}

long stack[1024];

struct {
    long * ptr;
    short segment;
} PACKED stack_start = { &stack[1024], 0x10 };
