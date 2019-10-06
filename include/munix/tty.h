#ifndef _MUNIX_TTY_H_
#define _MUNIX_TTY_H_

#define TTY_BUF_SIZE 1024

struct tty_queue {
    long data;
    long head;
    long tail;
    char buf[TTY_BUF_SIZE];
};



#endif
