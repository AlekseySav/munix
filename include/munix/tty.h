#ifndef _MUNIX_TTY_H_
#define _MUNIX_TTY_H_

#define TTY_BUF_SIZE 1024

#include <termios.h>

struct tty_queue {
    long data;
    long head;
    long tail;
    char buf[TTY_BUF_SIZE];
};

#define INC(a) ((a) = ((a) + 1) & (TTY_BUF_SIZE - 1))
#define CHARS(q) (((q).head - (q).tail) & (TTY_BUF_SIZE - 1))
#define GETCH(q, c) \
    do { c = (q).buf[(q).tail]; INC((q).tail); } while(0)
#define PUTCH(q, c) \
    do { (q).buf[(q).head] = c; INC((q).head); } while(0)

struct tty_struct {
    struct termios termios;
    struct tty_queue write_q;
    void (*write)(struct tty_struct * tty);
};

EXTERN struct tty_struct tty_table[];

EXTERN void tty_init(void);
EXTERN void con_init(void);

EXTERN int tty_write(unsigned channel, const char * buf, int nr);

EXTERN void con_write(struct tty_struct * tty);

#endif
