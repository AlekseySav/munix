#ifndef _MUNIX_TTY_H_
#define _MUNIX_TTY_H_

#undef TTY_BUF_SIZE
#define TTY_BUF_SIZE 1024

struct tty_queue {
    unsigned long head;
    unsigned long tail;
    char buf[TTY_BUF_SIZE];
};

#define INC(x) ((x) = ((x) + 1) & (TTY_BUF_SIZE - 1))
#define CHARS(q) (((q).head - (q).tail) & (TTY_BUF_SIZE - 1))
#define GETCH(q, c) do { c = (q).buf[(q).tail]; INC((q).tail); } while(0)

struct tty_struct {
    struct tty_queue write_q;
};

EXTERN void con_init(void);
EXTERN void con_write(struct tty_struct * tty);

#endif
