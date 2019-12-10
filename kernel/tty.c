#include <munix/tty.h>
#include <asm/segment.h>
#include <ctype.h>

#define O_FLAG(tty, flag)   ((tty)->termios.c_oflag & flag)

struct tty_struct tty_table[] = {
    {
        /* termios */ {
            OPOST | ONLCR,
            INIT_CC
        },
        { 0, 0, "" },
        con_write
    }
};

void tty_init(void)
{
    con_init();
}

int tty_write(unsigned channel, const char * buf, int nr)
{
    if(channel > 0 && nr < 0)
        return -1;

    struct tty_struct * tty = tty_table + channel;
    char c;
    const char * b = buf;

    while(nr--) {
        c = get_fs_byte(b);
        if(O_FLAG(tty, OPOST)) {
            if(c == '\r' && O_FLAG(tty, OCRNL))
                c = '\n';
            else if(c == '\n' && O_FLAG(tty, ONLRET))
                c = '\r';
            else if(c == '\t' && O_FLAG(tty, XTABS))
                c = ' ';
            else if(O_FLAG(tty, OLCUC) && islower(c))
                c = toupper(c);
            else if(c == '\n' && O_FLAG(tty, ONLCR)) {
                PUTCH(tty->write_q, c);
                c = '\r';
            }
        }
        PUTCH(tty->write_q, c);
        b++;
    }

    tty->write(tty);
}
