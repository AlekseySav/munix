#ifndef _MUNIX_KERNEL_H_
#define _MUNIX_KERNEL_H_

#ifdef _VA_LIST_
int vsprintf(char * buf, const char * fmt, va_list ap);
#endif

int printk(const char * fmt, ...);
void panic(const char * msg);

void tty_init(void);
void trap_init(void);
void sched_init(void);

#endif
