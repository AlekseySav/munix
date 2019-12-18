/*
 * fork.c copies current process to free task_table entity
 * source of forked process stored in new page, allocated by mm
 */

#include <munix/config.h>
#include <asm/system.h>
#include <munix/sched.h>
#include <string.h>
#include <errno.h>

static int pid = 0; /* same as init_task pid */

static int find_empty_process(void)
{
    unsigned i;
    /* get pid */
    for(i = 0; i < NR_TASKS; i++) {
        if(task_table[i]->pid == pid) {
            i = 0;
            pid++;
        }
        if(pid < 0) pid = 0;
    }

    /* get proc */
    for(i = 0; i < NR_TASKS; i++)
        if(!task_table[i])
            return i;
    
    return EGAIN;
}

/* arguments for sys_fork are pushed by system_call (see system_call.S) */
int sys_fork(long ebx, long ecx, long edx, long ebp, long esi, long edi, 
                long gs, long fs, long es, long ds,
                long eip, long cs, long eflags, long esp, long ss /* pushed by INT */ )
{
    int nr;
    struct task_struct * p;

    nr = find_empty_process();
    if(nr < 0) return EGAIN;

    p = (struct task_struct *)get_free_page();
    if(!p) return EGAIN;

    *p = *current;

    p->pid = pid;
    p->parent = current->pid; 
    p->tss.link = 0;
    p->tss.esp0 = (long)p + PAGE_SIZE;
    /* cr3 */
    p->tss.eip = eip;
    p->tss.eflags = eflags;
    p->tss.ss0 = 0x10;
    p->tss.eax = 0;                 /* this way fork returns 0 to kid */
    p->tss.ecx = ecx;
    p->tss.edx = edx;
    p->tss.ebx = ebx;
    p->tss.esp = esp;
    p->tss.ebp = ebp;
    p->tss.esi = esi;
    p->tss.edi = edi;
    p->tss.es = es;
    p->tss.cs = cs;
    p->tss.ss = ss;
    p->tss.fs = fs;
    p->tss.ds = ds;
    p->tss.gs = gs;
    p->tss.ldt = _LDT(nr);

    set_tss_desc(FIRST_TSS + (nr << 1), &p->tss);
    set_ldt_desc(FIRST_LDT + (nr << 1), &p->ldt);
    task_table[nr] = p;

    return pid;
}
