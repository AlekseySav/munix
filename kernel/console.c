#include "sys/const.h"
#include "asm/io.h"

#define SCREEN_START 0xb8000
#define SCREEN_END   0xc0000
#define LINES 25
#define COLUMNS 80

PRIVATE unsigned long pos;
PRIVATE unsigned long x, y;
PRIVATE unsigned char attr;

PRIVATE inline void gotoxy(unsigned int nx, unsigned int ny)
{
    if (nx >= COLUMNS || ny >= LINES) return;
    x = nx;
    y = ny;
    pos = SCREEN_START + ((y * 80 + x) << 1);
}

PRIVATE inline void set_cursor(void)
{
	cli();
	outb(14, 0x3d4);
	outb(0xff & ((pos - SCREEN_START) >> 9), 0x3d5);
	outb(15, 0x3d4);
	outb(0xff & ((pos - SCREEN_START) >> 1), 0x3d5);
	sti();
}

PRIVATE inline void print_char(char c)
{
    *(char *)pos = c;
    pos += 2;
    x++;
    if(x >= COLUMNS) 
    {
        x = 0;
        y++;
    }
}

PUBLIC void con_init(void)
{
    pos = *(unsigned long *)0x901fe;                // read cursor pos fram safe place
    gotoxy(pos & 0x00ff, (pos & 0xff00) >> 8);
    set_cursor();
}

PUBLIC void con_write(const char * ptr)
{
    while(*ptr != 0)
    {
        print_char(*ptr);
        ptr++;
    }
    set_cursor();
}
