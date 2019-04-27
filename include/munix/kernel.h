#ifndef _MUNIX_KERNEL_H_
#define _MUNIX_KERNEL_H_

#include <sys/const.h>
#include <stdarg.h>
#include <stddef.h>

EXTERN int vsprintf(char * buf, const char * fmt, va_list args);
EXTERN int printk(const char * fmt, ...);

#endif
