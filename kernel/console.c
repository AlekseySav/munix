#include <munix/kernel.h>

#define SCREEN_START    0xb8000
#define SCREEN_SIZE     4000
#define SCREEN_END      SCREEN_START + SCREEN_SIZE
#define LINES           25
#define COLUMNS         80

#ifdef NIL
#undef NIL
#endif
#define NIL             0x0720

PRIVATE u_long pos;
PRIVATE u_long x, y;
PRIVATE u_char attr;

EXTERN void keyh(void);

unsigned char keyboard_map[128] =
{
    0,  27, 
    '1', '2', '3', '4', '5', '6', '7', '8',  '9', '0', '-', '=', 
    '\b',	/* Backspace */ '\t',			/* Tab */
  'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 
  '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};


PRIVATE inline void set_cursor(void)
{
	cli();
	outb(14, 0x3d4);
	outb(0xff & ((pos - SCREEN_START) >> 9), 0x3d5);
	outb(15, 0x3d4);
	outb(0xff & ((pos - SCREEN_START) >> 1), 0x3d5);
	sti();
}

PRIVATE inline void scroll(int lines)
{
    int srl = lines * COLUMNS * 2;
    memcpy(SCREEN_START, SCREEN_START + srl, SCREEN_SIZE - srl);
    memsetw(SCREEN_END - srl, NIL, srl);
}

PRIVATE void gotoxy(u_int nx, u_int ny)
{
    if (nx >= COLUMNS) return;
    if (ny >= LINES)
    {
        scroll(ny + 1 - LINES);
        y = LINES - 1;
    }
    else y = ny;
    x = nx;
    pos = SCREEN_START + ((y * 80 + x) << 1);
}

//PRIVATE
 void print_char(char c)
{
    switch (c)
    {
        case '\n':
            gotoxy(0, y + 1);
            break;
        default:
            *(char *)pos = c;
            *(char *)(pos + 1) = attr;
            x++;
            if(x >= COLUMNS) gotoxy(0, y + 1);
            else pos += 2;
            break;
    }
}

PUBLIC void con_init(void)
{
    cli();
    int_set(0x21, (uint32_t)keyh);

    outb(0x11, 0x20);
    outb(0x11, 0xa0);

    outb(0x20, 0x21);
    outb(0x28, 0xa1);

    outb(0x00, 0x21);  
    outb(0x00, 0xa1);  

    outb(0x01, 0x21);
    outb(0x01, 0xa1);

    outb(0xff, 0x21);
    outb(0xff, 0xa1);

    outb(0xfd, 0x21);
    sti();

    pos = *(unsigned long *)0x901fe;                // read cursor pos fram safe place
    gotoxy(pos & 0x00ff, (pos & 0xff00) >> 8);
    set_cursor();
    attr = 0x07;                                    // white on black
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

PUBLIC void key_handler(int key) 
{
    print_char(key);
    set_cursor();
}
