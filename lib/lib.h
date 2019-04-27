/*
* lib.h
*
* this file should be used
* only in lib/
*/

#ifndef _LIB_H_
#define _LIB_H_

#ifndef LIB_SRC
    #define LIB_SRC 1
#endif

#include <ansi.h>

#include <sys/const.h>
#include <sys/types.h>

#include <asm/io.h>
#include <asm/system.h>

#include <munix/head.h>
#include <munix/kernel.h>
#include <munix/tty.h>

#include <memory.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>

#endif
