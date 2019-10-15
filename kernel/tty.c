#include "kernel.h"

#define _O_FLAG(tty, f) ((tty)->termios.c_oflag & (f))

#define O_POST(tty)     _O_FLAG(tty, OPOST)
#define O_CRNL(tty)     _O_FLAG(tty, OCRNL)
#define O_NLCR(tty)     _O_FLAG(tty, ONLCR)
#define O_NLRET(tty)    _O_FLAG(tty, ONLRET)
#define O_LCUC(tty)     _O_FLAG(tty, OLCUC)
#define O_XTABS(tty)    _O_FLAG(tty, XTABS)

PUBLIC struct tty_struct tty_table[] = {
    { 
        (struct termios) {
            OPOST | ONLCR,
            INIT_C_CC
        },
        { 0, 0, 0, "" },
        con_write
    }
};

PUBLIC void tty_init(void)
{
    con_init();
}

PUBLIC int tty_write(unsigned channel, const char * buf, int nr)
{
    if(channel > 0 || nr < 0)
        return -1;

    char c;
    const char * b = buf;
    struct tty_struct * tty = tty_table + channel;

    while(nr--) {
        c = get_fs_byte(b);

        if(O_POST(tty)) {
            if(c == '\r' && O_CRNL(tty))
                c = '\n';
            else if(c == '\n' && O_NLRET(tty))
                c = '\r';
            else if(c == '\t' && O_XTABS(tty))
                c = ' ';
            else if(c == '\n' && O_NLCR(tty)) {     // "\n" to "\n\r"
                PUTCH(tty->write_q, c);
                c = '\r';
            }
            else if(O_LCUC(tty))
                c = toupper(c);
        }

        PUTCH(tty->write_q, c);
        b++;
    }

    tty->write(tty);
    return (b - buf);
}
