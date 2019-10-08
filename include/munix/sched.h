#ifndef _MUNIX_SCHED_H_
#define _MUNIX_SCHED_H_

#include <sys/types.h>

struct stackframe_s {
    seg_t es;
    seg_t ds;
    
    reg_t di;
    reg_t si;
    reg_t fp;
    reg_t st;
    reg_t bx;
    reg_t dx;
    reg_t cx;
    reg_t retreg;
    reg_t retadr;
    reg_t pc;
    reg_t cs;
    reg_t psw;
    reg_t sp;
    reg_t ss;
};

struct task_struct {
    struct stackframe_s reg;
    long state;
};

#endif
