#ifndef _MUNIX_SCHED_H_
#define _MUNIX_SCHED_H_

#define HZ      100
#define LATCH   (1193182 / HZ)

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
    uint32_t map_base;
};

struct task_struct {
    struct tss_struct tss;
    struct table_struct ldt[3]; // nil, code, data
};

union task_union {
    struct task_struct task;    // tss at the top
    char stack[PAGE_SIZE];      // stack grows from bottom
};

EXTERN struct task_struct tasks[NR_TASKS];
EXTERN struct task_struct * current;

#define TSS_FIRST_ENTRY 4

#define _TSS(n) ((TSS_FIRST_ENTRY + (n << 1)) << 3)
#define _LDT(n) ((TSS_FIRST_ENTRY + 1 + (n << 1)) << 3)

#define ltr(n) ASM("ltr %%ax" :: "a" (_TSS(n)))
#define lldt(n) ASM("lldt %%ax" :: "a" (_LDT(n)))

#define switch_to(n) do { \
    struct { long a, b; } __tmp; \
    ASM VOLATILE( \
    "movw %%dx, %1\n" \
    "ljmp * %0\n" \
    ::  "m" (*&__tmp.a), "m" (*&__tmp.b), \
        "d" (_TSS(n))); \
} while(0)

#endif
