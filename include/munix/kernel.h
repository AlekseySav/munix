#ifndef _MUNIX_KERNEL_H_
#define _MUNIX_KERNEL_H_

#include <sys/types.h>
#include <sys/const.h>
#include <munix/asm.h>
#include <string.h>

EXTERN void con_init(void);
EXTERN void con_write(const char * ptr);

EXTERN void int_init(void);
EXTERN void int_set(int n, uint32_t handler);

#endif
