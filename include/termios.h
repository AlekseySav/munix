#ifndef _TERMIOS_H_
#define _TERMIOS_H_

#include <sys/types.h>

struct termios {
    tcflag_t c_oflag;   // output mode
};

// c_oflag bits
#define OPOST   0x0001  // perform output processing

// unused if OPOST is disabled
#define OCRNL   0x0002
#define ONLCR	0x0004
#define ONLRET  0x0008
#define OLCUC   0x0010
#define XTABS	0x0020

#endif
