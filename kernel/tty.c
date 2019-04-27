#include <sys/const.h>
#include <munix/tty.h>

PUBLIC struct tty_struct tty_table[] = {
    {
        (struct tty_queue) {
            0, 0, ""
        },
        con_write
    }
};

PUBLIC void tty_init(void)
{
    con_init();
}

PUBLIC int tty_write(unsigned channel, char * buf)
{
    struct tty_struct * tty = tty_table + channel;

    strcpy(buf, tty->write_q.buf);
    tty->write_q.tail = 0;
    tty->write_q.head = 0;
    while(tty->write_q.buf[tty->write_q.head++]);

    tty->write(tty);
}
