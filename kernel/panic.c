#include <munix/kernel.h>

void panic(const char * msg)
{
    printk("\033[1;34mKernel panic:\033[0m %s\n", msg);
    asm("cli; 1: jmp 1b");
}
