#ifndef _TERMIOS_H_
#define _TERMIOS_H_

#include <sys/types.h>

#define NCCS 1          // now, erase only is supported

struct termios {
    tcflag_t c_oflag;   // output mode
    cc_t c_cc[NCCS];
};

// c_oflag bits
#define OPOST   0x0001  // perform output processing

// unused if OPOST is disabled
#define OCRNL       0x0002
#define ONLCR	    0x0004
#define ONLRET      0x0008
#define OLCUC       0x0010
#define XTABS	    0x0020

// c_cc flags
#define VERASE      0

#endif
