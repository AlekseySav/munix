#ifndef _STRING_H_
#define _STRING_H_

#include <stddef.h>
#include <stdarg.h>

EXTERN size_t strlen(char * str);

EXTERN int strcpy(const char * dest, char * src);

EXTERN int vsprintf(char * buf, const char * fmt, va_list args);
EXTERN int sprintf(char * buf, const char * fmt, ...);

#endif
