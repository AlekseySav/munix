/*
 * munix/unistd.h
 * 
 * NOTE: munix/unistd.h exports some features,
 * that help to define munix syscalls (see lib/alarm.c, etc...)
 * Declarations of munix syscalls are contained in include/unistd.h
 * So, as usual, this routines are used in kernel code only
 */

#ifndef _MUNIX_UNISTD_H_
#define _MUNIX_UNISTD_H_

#include <errno.h>

#define __NR_setup      0
#define __NR_fork       2
#define __NR_write      4
#define __NR_getpid     20
#define __NR_alarm	    27
#define __NR_pause	    29
#define __NR_getppid    64

#define syscall0(type, name) \
type name(void) \
{ \
    type res; \
    asm volatile("int $0x80" \
        :   "=a" (res) \
        :   "a" (__NR_##name)); \
    if(res >= 0) \
        return res; \
    errno = -res; \
    return -1; \
}

#define syscall1(type, name, t1, a1) \
type name(t1 a1) \
{ \
    type res; \
    asm volatile("int $0x80" \
        :   "=a" (res) \
        :   "a" (__NR_##name), \
            "b" (a1)); \
    if(res >= 0) \
        return res; \
    errno = -res; \
    return -1; \
}

#define syscall2(type, name, t1, a1, t2, a2) \
type name(t1 a1, t2 a2) \
{ \
    type res; \
    asm volatile("int $0x80" \
        :   "=a" (res) \
        :   "a" (__NR_##name), \
            "b" (a1), \
            "c" (a2)); \
    if(res >= 0) \
        return res; \
    errno = -res; \
    return -1; \
}

#define syscall3(type, name, t1, a1, t2, a2, t3, a3) \
type name(t1 a1, t2 a2, t3 a3) \
{ \
    type res; \
    asm volatile("int $0x80" \
        :   "=a" (res) \
        :   "a" (__NR_##name), \
            "b" (a1), \
            "c" (a2), \
            "d" (a3)); \
    if(res >= 0) \
        return res; \
    errno = -res; \
    return -1; \
}

#endif
