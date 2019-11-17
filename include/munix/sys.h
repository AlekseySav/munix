#ifndef _MUNIX_SYS_H_
#define _MUNIX_SYS_H_

#include <sys/types.h>

void sys_init(void);

int sys_check(void);
int sys_display(const char * msg);

EXTERN fn_ptr sys_call_table[];

#endif
