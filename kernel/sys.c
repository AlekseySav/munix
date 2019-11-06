#include "kernel.h"

fn_ptr sys_call_table[] = {
    check_syscalls
};

int check_syscalls(void)
{
    printk("system call\n");
}
