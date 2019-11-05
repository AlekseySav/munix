#ifndef _ANSI_H_
#include <ansi.h>
#endif

#ifndef _KERNEL_H_
#define _KERNEL_H_

#include <munix/config.h>

#include <sys/const.h>
#include <sys/types.h>

#include <stddef.h>
#include <stdarg.h>
#include <termios.h>
#include <memory.h>
#include <ctype.h>

#include <munix/head.h>
#include <munix/tty.h>
#include <munix/sched.h>
#include <munix/mm.h>

#include <asm/system.h>
#include <asm/segment.h>
#include <asm/io.h>

int printk(const char * fmt, ...);
void panic(const char * msg);

void tty_init(void);
void trap_init(void);
void sched_init(void);

#endif
