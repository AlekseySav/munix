#include <sys/const.h>
#include <sys/types.h>
#include <asm/system.h>		// for cli(), sti()
#include <asm/io.h>			// for outb()
#include <memory.h>			// for memcpyw, memsetw

#define SCREEN_START 0xb8000
#define COLUMNS 80
#define LINES 25

#define SCREEN_SIZE LINES * COLUMNS * 2
#define SCREEN_END SCREEN_START + SCREEN_SIZE
#define BOTTOM LINES
#define NULL_ATTR 0x0720

PRIVATE unsigned long origin = SCREEN_START;
PRIVATE unsigned long pos;
PRIVATE uint8_t x, y;
PRIVATE uint8_t attr = 0x07;

PRIVATE inline void gotoxy(uint8_t new_x, uint8_t new_y)
{
    if(new_x >= COLUMNS || new_y >= LINES)
        return;

    x = new_x;
    y = new_y;
    pos = origin + ((y * COLUMNS + x) << 1);
}

PRIVATE inline void set_origin(void)
{
	cli();
	outb(12, 0x3d4);
	outb(0xff & ((origin - SCREEN_START) >> 9), 0x3d5);
	outb(13, 0x3d4);
	outb(0xff & ((origin - SCREEN_START) >> 1), 0x3d5);
	sti();
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

PRIVATE void scroll(int lines)
{
	int len = (lines * COLUMNS) << 1;
	memcpyw(origin, origin - len, SCREEN_SIZE);
	if(len >= 0) memsetw(origin, NULL_ATTR, len >> 1);
	else memsetb(origin + SCREEN_SIZE + len, 0, -len);
}

PRIVATE inline void scrup(void)
{
	scroll(-1);
}

PRIVATE inline void scrdown(void)
{
	scroll(1);
}

PRIVATE void lf(void)
{
	if(y <= BOTTOM) {
		y++;
		pos += COLUMNS << 1;
		return;
	}
	scrup();
}

PRIVATE void li(void)
{
	if(y > 0) {
		y--;
		pos -= COLUMNS << 1;
		return;
	}
	scrdown();
}

PRIVATE void cr(void)
{
	pos -= x << 1;
	x = 0;
}

PRIVATE void del(void)
{
	if(x) {
		x--;
		pos -= 2;
		*(uint16_t *)pos = NULL_ATTR;
	}
}

PUBLIC void con_init(void)
{
    register long _pos = *(unsigned *)(0x90000 + 510);		// cursor pos (see boot/boot.s)
    short _x = _pos & 0xff;
    short _y = (_pos & 0xff00) >> 8;
	gotoxy(_x, _y);
}



PUBLIC void async_write(const char * ptr)
{
    while(*ptr) {
		if(*ptr == '\n') {
			lf();
			cr();
		}
		else {
			*(char *)pos = *ptr;
			*(char *)(pos + 1) = 0x07;
			x++;
			if(x >= COLUMNS) {
				y++;
				x = 0;
				if(y >= LINES) {
					scrup();
					y--;
				}
			}
		pos += 2;
		}
		ptr++;
	}
	set_cursor();
}
