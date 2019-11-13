#include "kernel.h"

EXTERN void sys_call(void);

PUBLIC fn_ptr sys_call_table[] = {
    check_syscalls
};

PUBLIC void sys_init(void)
{
    set_intr_gate(0x80, sys_call);
}

PUBLIC int check_syscalls(void)
{
    return printk("system call\n");
}
