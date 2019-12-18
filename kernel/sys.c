#include <munix/config.h>
#include <asm/system.h>
#include <munix/head.h>
#include <munix/kernel.h>
#include <stddef.h>

void system_call(void);

extern int sys_setup(void);
extern int sys_fork(void);
extern int sys_getpid(void);
extern int sys_alarm(long seconds);
extern int sys_pause(void);
extern int sys_getppid(void);

int sys_write(int fd, char * buf, int nr)
{
    return printk("%s", buf);
}

fn_ptr syscall_table[] = {
    (fn_ptr)sys_setup,
    (fn_ptr)NULL,
    (fn_ptr)sys_fork,
    (fn_ptr)NULL,
    (fn_ptr)sys_write,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)sys_getpid,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)sys_alarm,
    (fn_ptr)NULL,
    (fn_ptr)sys_pause,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)NULL,
    (fn_ptr)sys_getppid
};

void sys_init(void)
{
    set_system_gate(0x80, system_call);
}
