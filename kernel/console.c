#include <munix/config.h>
#include <munix/tty.h>
#include <sys/types.h>
#include <asm/io.h>
#include <asm/system.h>
#include <ctype.h>

#define COLUMNS         80
#define LINES           25

#define SCREEN_START    0xb8000
#define SCREEN_SIZE     (LINES * COLUMNS * 2)

#define NPAR        16      // parameter max
#define TAB         8

static uint8_t x, y;
static uint8_t attr;
static uint8_t sx, sy;

static unsigned pos;
static unsigned state;

static unsigned par[NPAR];
static unsigned npar;

static inline void gotoxy(uint8_t nx, uint8_t ny)
{
    if(nx >= COLUMNS || ny >= LINES)
        return;
    x = nx;
    y = ny;
    pos = SCREEN_START + (((y * 80) + x) << 1);
}

static inline void set_cursor(void)
{
    uint16_t cursor = y * 80 + x;

    cli();
    outb(0x0e, 0x3d4);
    outb((cursor >> 8), 0x3d5);
    outb(0x0f, 0x3d4);
    outb((cursor & 0xff), 0x3d5);
    sti();
}

static inline void scrup(void)
{
    asm("cld; rep; movsb;"
        ::  "D" (SCREEN_START), 
            "S" (SCREEN_START + (COLUMNS * 2)),
            "c" (SCREEN_SIZE - (COLUMNS * 2)));
    asm("cld; rep; stosw" 
        ::  "D" (SCREEN_START + SCREEN_SIZE - (COLUMNS * 2)), 
            "a" (0x0720), 
            "c" ((COLUMNS * 2)));
}

static void nl(void)        // \n
{
    if(y + 1 < LINES) {
        y++;
        pos += COLUMNS << 1;
        return;
    }
    scrup();
}

static void cr(void)        // \r
{
    pos -= x << 1;
    x = 0;
}

static void bs(void)        // \b
{
    if(x) {
        x--;
        pos -= 2;
    }
}

static inline void save_cursor(void)
{
    sx = x;
    sy = y;
}

static inline void restore_cursor(void)
{
    x = sx;
    y = sy;
}

static inline void nlcr(void)
{
    nl();
    cr();
}

// set attribute
static inline void csi_m(void)
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

// display one char
static inline void print_char(struct tty_struct * tty, char c)
{
    switch(state) {
        case 0:
            if(isprint(c)) {
                asm("movb %0, %%al\n\t"
                    "movb %1, %%ah\n\t"
                    "movw %%ax, %2\n\t"
                    :: "m" (c), "m" (attr), "m" (*(short *)pos) 
                    : "ax");
                pos += 2;
                x++;
                if(x >= COLUMNS) nlcr();
            }
            else if(c == ERASE_CHAR(tty)) {
                bs();
                *(short *)pos = 0x0720;
            }
            else if(c == '\t') {
                c = 8 - (x % TAB);
                x += c;
                pos += c << 1;
                if(x >= COLUMNS) nlcr();
            }
            else if(c == '\n' || c == '\v' || c == '\f')
                nl();
            else if(c == '\b')
                bs();
            else if(c == '\r')
                cr();
            else if(c == '\033')
                state = 1;
            break;
        case 1:                 // \033X - esc sequence
            if(c == '[') {
                state = 2;
                break;
            }
            else if(c == '7')
                save_cursor();
            else if(c == '8')
                restore_cursor();
            break;
        case 2:                 // clear parameter list
            for(npar = 0; npar < NPAR; npar++)
                par[npar] = 0;
            npar = 0;
            state = 3;
        case 3:                 // read parameters
            if(isdigit(c)) {
                par[npar] = par[npar] * 10 + c - '0';
                break;
            }
            if(c == ';') {
                npar++;
                if(npar >= NPAR)
                    state = 0;
                break;
            }
            state = 4;
        case 4:
            if(c == 'm')
                csi_m();
            state = 0;
            break;
    }
}

void con_init(void)
{
    gotoxy(*(uint8_t *)(INITSEG * 0x10), *(uint8_t *)(INITSEG * 0x10 + 1));
    attr = 0x07;
}

void con_write(struct tty_struct * tty)
{
    int nr;
    char c;
    
    nr = CHARS(tty->write_q);
    while(nr--) {
        GETCH(tty->write_q, c);
        print_char(tty, c);
    }
    set_cursor();
}
