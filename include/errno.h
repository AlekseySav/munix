#ifndef _ERRNO_H_
#define _ERRNO_H_

extern int errno;

#ifdef _KERNEL
    #define _SIGN -
#else
    #define _SIGN
#endif

#define ERROR       99      /* number of errors */

#define EGAIN       (_SIGN 11)

#endif
