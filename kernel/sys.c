#include <munix/config.h>
#include <asm/system.h>
#include <munix/head.h>
#include <munix/kernel.h>
#include <stddef.h>

void system_call(void);

int sys_setup(void);
int sys_fork(void);

int sys_write(const char * msg)
{
    return printk("%s", msg);
}

fn_ptr syscall_table[] = {
    (fn_ptr)sys_setup,
    (fn_ptr)sys_write,
    (fn_ptr)sys_fork
};

void sys_init(void)
{
    set_system_gate(0x80, system_call);
}
