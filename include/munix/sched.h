#ifndef _MUNIX_SCHED_H_
#define _MUNIX_SCHED_H_

#include <sys/types.h>

#define HZ      1193182
#define LATCH   (50000 / HZ)

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

struct tss_struct {
    int32_t link;
    int32_t esp0;
    int32_t ss0;
    int32_t esp1;
    int32_t ss1;
    int32_t esp2;
    int32_t ss2;

    reg_t cr3;
    reg_t eip;
    reg_t eflags;

    reg_t eax;
    reg_t ecx;
    reg_t edx;
    reg_t ebx;
    reg_t esp;
    reg_t ebp;
    reg_t esi;
    reg_t edi;

    reg_t es;
    reg_t cs;
    reg_t ss;
    reg_t ds;
    reg_t fs;
    reg_t gs;

    uint32_t ldtr;
    uint32_t offset;
};

struct task_struct {
    struct stackframe_s reg;
    long state;
    long ptr;
};

EXTERN struct task_struct tasks[NR_TASKS];
EXTERN struct task_struct * current;

#define TSS_FIRST_ENTRY 4

#define switch_to(n_0) do { \
    struct { long a, b; } __tmp; \
    ASM( \
    "movw %%dx, %1\n" \
    "ljmp %0\n" \
    ::  "m" (*&__tmp.a), "m" (*&__tmp.b), \
        "d" (TSS_FIRST_ENTRY << 3)); \
} while(0)

#endif
