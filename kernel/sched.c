#include <munix/config.h>
#include <munix/sched.h>
#include <asm/system.h>
#include <asm/io.h>
#include <signal.h>

long volatile jiffies = 0;

extern void timer_intr(void);

static union task_union init_task = {
    {
        TASK_RUNNING, 
        0, 0, 
        0, 15, 0, 0, 
        /* ldt */ { 
            { 0, 0 },                       // NULL
            { 0x000007ff, 0x00c0fa00 },     // code
            { 0x000007ff, 0x00c0f200 }      // data
        },
        /* tss */ {
            0,
            (long)&init_task + PAGE_SIZE, 0x10,
            0, 0, 0, 0, 
            (long)&pg_dir, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0x17, 0x0f, 0x17, 0x17, 0x17, 0x17,
            _LDT(0), 0, 0
        }
    }
};

struct task_struct * task_table[NR_TASKS] = { &init_task.task };
struct task_struct * current;

static void shedule(void)
{
    unsigned i, next;
    int c = -1;
    struct task_struct * p;

    for(i = 0; i < NR_TASKS; i++) {
        p = task_table[i];
        if(!p) continue;
        if(p->alarm && p->alarm < jiffies)
            p->signal |= SIG(SIGALRM);
        if(p->signal && p->state == TASK_PAUSE)
            p->state = TASK_RUNNING;
    }

    for(i = 0; i < NR_TASKS; i++) {
        p = task_table[i];
        if(!p) continue;
        if(p->state != TASK_RUNNING)
            continue;

        if(p->counter > c) {
            c = p->counter;
            next = i;
        }
        else p->counter += p->priority;
    }

    switch_to(next);
}

void do_timer(void)
{
    jiffies++;
    if(--current->counter > 0) return;
    shedule();
}

int sys_pause(void)
{
    current->state = TASK_PAUSE;
    shedule();
    return 0;
}

int sys_alarm(long seconds)
{
    if(seconds > 0)
        current->alarm = jiffies + HZ * seconds;
    else current->alarm = 0;
    return seconds;
}

int sys_getpid(void)
{
    return current->pid;
}

int sys_getppid(void)
{
    return current->parent;
}

void sched_init(void)
{
    set_ldt_desc(FIRST_LDT, &init_task.task.ldt);   /* NOTE! use FIRST_LDT + (nr << 1); not _LDT(nr) */
    set_tss_desc(FIRST_TSS, &init_task.task.tss);   /* ... */
    ltr(0);
    lldt(0);
    current = &init_task.task;


	outb(52, 0x43);		                            // binary, mode 3, lobyte/hibyte
	outb(LATCH & 0xff, 0x40);	                    // lobyte
	outb(LATCH >> 8, 0x40);	                        // hibyte

	set_intr_gate(0x20, &timer_intr);
	outb(inb(0x21) & ~0x01, 0x21);
}
