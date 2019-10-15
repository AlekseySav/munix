#include "kernel.h"

#define SCREEN_START    0xb8000
#define SCREEN_END      0xc0000
#define COLUMNS         80
#define LINES           25
#define NPAR            16

#define SCREEN_SIZE     (LINES * COLUMNS * 2)

PRIVATE uint32_t origin = SCREEN_START;
PRIVATE uint32_t end = SCREEN_START + SCREEN_SIZE;
PRIVATE uint32_t lines = LINES, columns = COLUMNS;

PRIVATE uint32_t state = 0;
PRIVATE uint8_t attr = 0x07;

PRIVATE uint32_t pos;
PRIVATE uint8_t x, y;

PRIVATE unsigned npar, par[NPAR];

PRIVATE void gotoxy(uint8_t nx, uint8_t ny)
{
    if(nx >= COLUMNS || ny >= LINES)
        return;

    x = nx;
    y = ny;
    pos = origin + ((y * COLUMNS + x) << 1);
}

PRIVATE INLINE void set_cursor(void)
{
	cli();
	outb_p(14, 0x3d4);
	outb_p(0xff & ((y * 80 + x) >> 8), 0x3d5);
	outb_p(15, 0x3d4);
	outb_p(0xff & (y * 80 + x), 0x3d5);
	sti();
}

PRIVATE INLINE void set_origin(void)
{
	cli();
	outb_p(12, 0x3d4);
	outb_p(0xff & ((origin - SCREEN_START) >> 9), 0x3d5);
	outb_p(13,0x3d4);
	outb_p(0xff & ((origin - SCREEN_START) >> 1), 0x3d5);
	sti();
}

PRIVATE INLINE void scrup(void)
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

PRIVATE INLINE void scrdown(void)
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

PRIVATE void ri(void)
{
    if(y > 0) {
        y--;
        pos -= columns << 1;
        return;
    }
    scrdown();
}

PRIVATE uint8_t sx, sy;

PRIVATE INLINE void save_cursor(void)
{
    sx = x;
    sy = y;
}

PRIVATE INLINE void restore_cursor(void)
{
    x = sx;
    y = sy;
}

// clear screen ...
PRIVATE INLINE void csi_J(unsigned type)
{
    unsigned start, len;

    if(type == 0) {
        start = pos;
        len = (end - pos) >> 1;
    }
    else if(type == 1) {
        start = origin;
        len = (pos - origin) >> 1;
    }
    else if(type == 2) {
        start = origin;
        len = (end - origin) >> 1;
    }

    memsetw(start, 0x0720, len);
}

// clear line ...
PRIVATE INLINE void csi_K(unsigned type)
{
    unsigned start, len;

    if(type == 0) {
        start = pos;
        len = columns - x;
    }
    else if(type == 1) {
        start = pos - (x << 1);
        len = x;
    }
    else if(type == 2) {
        start = pos - (x << 1);
        len = columns;
    }

    memsetw(start, 0x0720, len);
}

// delete chars
PRIVATE INLINE void csi_P(unsigned nr)
{
    if(nr > x)
        nr = x;
    if(!nr) nr++;
    
    memcpyw(pos - (nr << 1), pos, columns - x);
    memsetw(pos + ((columns - x - nr) << 1), 0x0720, nr);
}

// insert chars
PRIVATE INLINE void csi_at(unsigned nr)
{
    if(nr > columns - x)
        nr = columns - x;
    if(!nr) nr++;
    
    memcpyw(pos + (nr << 1) - 2, pos - 2, columns - x);
    memsetw(pos, 0x0720, nr);
}

// set attribute
PRIVATE INLINE void csi_m(void)
{
    for(unsigned i = 0; i <= npar; i++) {
        if(par[i] == 0)
            attr = 0x07;
        else if(par[i] == 1)
            attr |= 8;
        else if(par[i] == 2)
            attr &= ~8;
        else if(par[i] == 7 || par[i] == 27)
            attr = 0xff - attr;
        else if(par[i] >= 30 && par[i] <= 37) {
            attr &= ~7;
            attr |= (par[i] - 30);
        }
        else if(par[i] == 38) {
            attr &= ~7;
            i++;
            if(par[i] == 5)
                attr |= par[++i];
            else if(par[i] == 2)
                attr |= ((par[++i] << 2) | (par[++i] << 1) | par[++i]) & 7;
        }
        else if(par[i] == 39)
            attr |= 7;
        else if(par[i] >= 40 && par[i] <= 47) {
            attr &= ~0x70;
            attr |= (par[i] - 40) << 1;
        }
        else if(par[i] == 48) {
            attr &= ~0x70;
            i++;
            if(par[i] == 5)
                attr |= par[++i];
            else if(par[i] == 2)
                attr |= ((((par[++i] << 2) | (par[++i] << 1) | par[++i])) & 7) << 1;
        }
        else if(par[i] == 49)
            attr &= ~0x70;
    }
}

PUBLIC void con_init(void)
{
    gotoxy(*(uint8_t *)(0x90000+510), *(uint8_t *)(0x90000+511));
}

PUBLIC void con_write(struct tty_struct * tty)
{
    char c;
    int nr = CHARS(tty->write_q);

    while(nr--) {
        GETCH(tty->write_q, c);
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
                else if(c == ERASE_CHAR(tty)) {
                    if(x) {
                        x--;
                        pos -= 2;
                        *(short *)pos = 0x0720;
                    }
                }
                else if(c == '\033')
                    state = 1;
                break;
            case 1:             // "\033X" - esc
                state = 0;
                if(c == '[')
                    state = 2;
                else if(c == 'E') {
                    lf();
                    cr();
                }
                else if(c == 'D')
                    lf();
                else if(c == 'M')
                    ri();
                else if(c == '7')
                    save_cursor();
                else if(c == '8')
                    restore_cursor();
                break;
            case 2:
                // clear parameter list
                for(npar = 0; npar < NPAR; npar++)
                    par[npar] = 0;
                npar = 0;
                state = 3;
            case 3:         // "\033[1;2;3x" - csi
                if(isdigit(c)) {
                    par[npar] = par[npar] * 10 + c - '0';
                    break;
                }
                if(c == ';' && npar < NPAR - 1) {
                    npar++;
                    break;
                }
                state = 4;
            case 4:
                state = 0;
                switch(c) {
                    case 'A':   // move up
                        if(!par[0]) par[0]++;
                        gotoxy(x, y - par[0]);
                        break;
                    case 'B':   // move down
                        if(!par[0]) par[0]++;
                        gotoxy(x, y + par[0]);
                        break;
                    case 'C':   // move right
                        if(!par[0]) par[0]++;
                        gotoxy(x + par[0], y);
                        break;
                    case 'D':   // move left
                        if(!par[0]) par[0]++;
                        gotoxy(x - par[0], y);
                        break;
                    case 'E':
                        if(!par[0]) par[0]++;
                        gotoxy(0, y + par[0]);
                        break;
                    case 'F':
                        if(!par[0]) par[0]++;
                        gotoxy(0, y - par[0]);
                        break;
                    case 'G':
                        if(!par[0]) par[0]++;
                        gotoxy(par[0], y);
                        break;
                    case 'H': case 'f':
                        if(par[0]) par[0]--;
                        if(par[1]) par[1]--;
                        gotoxy(par[1], par[0]);
                        break;
                    case 'J':
                        csi_J(par[0]);
                        break;
                    case 'K':
                        csi_K(par[0]);
                        break;
                    case 'P':
                        csi_P(par[0]);
                        break;
                    case '@':
                        csi_at(par[0]);
                        break;
                    case 'm':
                        csi_m();
                        break;
                    case 's':
                        save_cursor();
                        break;
                    case 'u':
                        restore_cursor();
                        break;
                }
                break;
        }
    }

    set_cursor();
}
