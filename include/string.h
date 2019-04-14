#ifndef _STRING_H_
#define _STRING_H_

#include <stddef.h>

EXTERN size_t strlen(char * str);

// EXTERN int vsprintf(char * buf, const char * fmt, va_list args);
// is defined in kernel/vsprintf.c (declared in munix/kernel.h)

EXTERN int sprintf(char * buf, const char * fmt, ...);

#endif
