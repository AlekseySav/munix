#include "kernel.h"

EXTERN void sys_call(void);

PUBLIC fn_ptr sys_call_table[] = {
    (fn_ptr)sys_check,
    (fn_ptr)sys_display
};

PUBLIC void sys_init(void)
{
    set_system_gate(0x80, sys_call);
}

PUBLIC int sys_check(void)
{
    return printk("test_util... ok\n");
}

PUBLIC int sys_display(const char * msg)
{
    return printk(msg);
}
