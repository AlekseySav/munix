#include "kernel.h"

#define SCREEN_START    0xb8000
#define SCREEN_END      0xc0000
#define COLUMNS         80
#define LINES           25

#define SCREEN_SIZE     (LINES * COLUMNS * 2)

PRIVATE uint32_t origin = SCREEN_START;
PRIVATE uint32_t end = SCREEN_START + SCREEN_SIZE;
PRIVATE uint32_t lines = LINES, columns = COLUMNS;

PRIVATE uint32_t state = 0;
PRIVATE uint8_t attr = 0x07;

PRIVATE uint32_t pos;
PRIVATE uint8_t x, y;

PRIVATE inline void gotoxy(uint8_t nx, uint8_t ny)
{
    if(x >= COLUMNS || y >= LINES)
        return;

    x = nx;
    y = ny;
    pos = origin + ((y * COLUMNS + x) << 1);
}

PRIVATE void set_cursor(void)
{
	cli();
	outb(14, 0x3d4);
	outb(0xff & ((y * 80 + x) >> 8), 0x3d5);
	outb(15, 0x3d4);
	outb(0xff & (y * 80 + x), 0x3d5);
	sti();
}

PRIVATE void set_origin(void)
{
	cli();
	outb(12, 0x3d4);
	outb(0xff & ((origin - SCREEN_START) >> 9), 0x3d5);
	outb(13,0x3d4);
	outb(0xff & ((origin - SCREEN_START) >> 1), 0x3d5);
	sti();
}

PRIVATE inline void scrup(void)
{
    origin += columns << 1;
    pos += columns << 1;
    end += columns << 1;

    if(end > SCREEN_END) {
        memcpyb(SCREEN_START, origin, SCREEN_SIZE);
		end -= origin - SCREEN_START;
		origin = SCREEN_START;
        gotoxy(0, 0);
        memsetw(origin + SCREEN_SIZE, 0x0720, end - SCREEN_SIZE);
    }
    else
        memsetw(origin + SCREEN_SIZE - (columns << 1), 0x0720, columns << 1);
    set_origin();
}

PRIVATE inline void scrdown(void)
{
    if(origin == 0) {
        memcpyb(origin + columns << 1, origin, SCREEN_SIZE);
        memsetw(SCREEN_START, 0x0720, SCREEN_START + columns << 1);
    }
    else {
        origin -= columns << 1;
        set_origin();
    }
}

PRIVATE void cr(void)
{
	pos -= x << 1;
	x = 0;
}

PRIVATE void lf(void)
{
	if (y + 1 < lines) {
		y++;
		pos += columns << 1;
		return;
	}
	scrup();
}

PUBLIC void con_init(void)
{
    gotoxy(*(uint8_t *)(0x90000+510), *(uint8_t *)(0x90000+511));
}

PUBLIC void __putc(char c)
{
    *(char *)pos++ = c;
    *(uint8_t *)pos++ = attr;
    x++;
}

PUBLIC void con_write(const char * str)
{
    char c;

    while(c = *str++) {
        switch(state) {
            case 0:
                if(isprint(c)) {
                    if(x >= columns) {
                        cr();
                        lf();
                    }
                    ASM("movb attr, %%ah\n"
                        "movw %%ax, %1\n"
                        :: "a" (c), "m" (*(short *)pos));
                    pos += 2;
                    x++;
                }
                else if(c == '\t') {
                    c = 8 - (x % 8);
                    x += c;
                    pos += c << 1;
                    if(x >= columns) {
                        cr();
                        lf();
                    }
                }
                else if(c == '\b') {
                    if(x) {
                        x--;
                        pos -= 2;
                    }
                }
                else if(c == '\n' || c == '\v' || c == '\f')
                    lf();
                else if(c == '\r')
                    cr();
                else if(c == '\033')
                    state = 1;
                break;
            default:
                state = 0;
                break;
        }
    }

    set_cursor();
}
