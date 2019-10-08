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

#include <asm/system.h>
#include <asm/segment.h>
#include <asm/io.h>

EXTERN int printk(const char * fmt, ...);

#endif
