/*
 * fork.c copies process from task_table[0] to task_table[1].
 * source of forked process stored in fork_task
 */

#include <asm/system.h>
#include <munix/sched.h>
#include <string.h>

static struct task_struct fork_task;
static char fork_stack[4096];

int sys_fork(long eip)
{
    struct task_struct * task = task_table[0];
    memcpy(&fork_task, task, sizeof(struct task_struct));

    fork_task.tss.eax = 0;          /* this way sys_fork returns 0 for child */
    fork_task.tss.eip = 0x40ed;     /* i set it by hand when debugging */
    task_table[1] = &fork_task;
    
    set_tss_desc(FIRST_TSS + 2, &fork_task.tss);
    set_ldt_desc(FIRST_LDT + 2, &fork_task.ldt);

    return 1;                       /* this way fork returns non-zero for parent */
}
