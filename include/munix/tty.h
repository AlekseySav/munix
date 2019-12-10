#ifndef _MUNIX_TTY_H_
#define _MUNIX_TTY_H_

#include <termios.h>

#define TTY_BUF_SIZE    1024    // must be power of 2

struct tty_queue
{
    unsigned long head;
    unsigned long tail;
    char buf[TTY_BUF_SIZE];
};

#define INC(a)      ((a) = ((a) + 1) & (TTY_BUF_SIZE - 1))
#define CHARS(q)    (((q).head - (q).tail) & (TTY_BUF_SIZE - 1))

#define PUTCH(q, c) do { (q).buf[(q).head] = (c); INC((q).head); } while(0)
#define GETCH(q, c) do { (c) = (q).buf[(q).tail]; INC((q).tail); } while(0)

struct tty_struct
{
    struct termios termios;
    struct tty_queue write_q;
    void (* write)(struct tty_struct * tty);
};

#define INIT_CC "\177"

#define ERASE_CHAR(tty) (tty->termios.c_cc[VERASE])

extern struct tty_struct tty_table[];

int tty_write(unsigned channel, const char * buf, int nr);

void con_init(void);
void con_write(struct tty_struct * tty);

#endif
