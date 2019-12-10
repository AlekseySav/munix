#ifndef _ASM_SEGMENT_H_
#define _ASM_SEGMENT_H_

#define get_fs_byte(ptr) ({ \
    unsigned register char _v; \
    asm("movb %%fs:%1, %0" : "=r" (_v) : "m" (*ptr)); \
    _v; })

#endif
