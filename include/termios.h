#ifndef _TERMIOS_H_
#define _TERMIOS_H_

typedef unsigned long tcflag_t;
typedef unsigned char cc_t;

#define NCCS 1

struct termios
{
    tcflag_t c_oflag;
    cc_t c_cc[NCCS];
};

// c_oflag
#define OPOST       0x0001      // perform output
#define OCRNL       0x0002
#define ONLCR       0x0004
#define ONLRET      0x0008
#define OLCUC       0x0010
#define XTABS       0x0020

// c_cc
#define VERASE      0

#endif
