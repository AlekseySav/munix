#ifndef _MUNIX_SCHED_H_
#define _MUNIX_SCHED_H_

#include <sys/types.h>
#include <munix/config.h>
#include <munix/head.h>
#include <munix/mm.h>

#define HZ      100
#define LATCH   (1193182 / HZ)

#define TASK_RUNNING    0   // normal task

struct tss_struct
{
    uint32_t link;          // 16 high bits zero
    uint32_t esp0;
    uint32_t ss0;           // 16 high bits zero
    uint32_t esp1;
    uint32_t ss1;           // 16 high bits zero
    uint32_t esp2;
    uint32_t ss2;           // 16 high bits zero
    uint32_t cr3;
    uint32_t eip;
    uint32_t eflags;
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
    uint32_t es;            // 16 high bits zero
    uint32_t cs;            // 16 high bits zero
    uint32_t ss;            // 16 high bits zero
    uint32_t ds;            // 16 high bits zero
    uint32_t fs;            // 16 high bits zero
    uint32_t gs;            // 16 high bits zero
    uint32_t ldt;           // 16 high bits zero
    uint16_t bitmap;
    uint16_t trace;         // 15 high bits zero
};

struct task_struct
{
    int state;
    int pid;
    int counter;
    int priority;
    struct table_entry ldt[3];
    struct tss_struct tss;
};

union task_union
{
    struct task_struct task;
    char stack[PAGE_SIZE];
};

extern struct task_struct * task_table[NR_TASKS];
extern struct task_struct * current;

/* tss[0] is gdt[4], ldt[0] = gdt[5], tss[1] = gdt[6], etc */
#define FIRST_TSS       4
#define FIRST_LDT       (FIRST_TSS + 1)
#define _TSS(n)         ((FIRST_TSS + (n << 1)) << 3)
#define _LDT(n)         ((FIRST_LDT + (n << 1)) << 3)

#define ltr(n)          asm("ltr %%ax" :: "a" (_TSS(n)))
#define lldt(n)         asm("lldt %%ax" :: "a" (_LDT(n)))

#define switch_to(n) do { \
    struct { long a, b; } __tmp; \
    asm volatile( \
        "movw %%dx, %1\n\t" \
        "ljmp * %0\n\t" \
        "clts"\
        ::  "m" (*&__tmp.a), "m" (*&__tmp.b), \
            "d" (_TSS(n))); \
    } while(0)

#endif
