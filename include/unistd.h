#ifndef _UNISTD_H_
#define _UNISTD_H_

#define __NR_setup      0
#define __NR_write     1
#define __NR_fork       2

#define syscall0(type, name) \
type name(void) \
{ \
    type res; \
    asm("int $0x80" \
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
    asm("int $0x80" \
        :   "=a" (res) \
        :   "a" (__NR_##name), \
            "b" (a1)); \
    if(res >= 0) \
        return res; \
    errno = -res; \
    return -1; \
}

int fork(void);

#endif
