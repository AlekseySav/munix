#ifndef _MUNIX_SYS_H_
#define _MUNIX_SYS_H_

#include <sys/types.h>

void sys_init(void);

int check_syscalls(void);

EXTERN fn_ptr sys_call_table[];

#endif
