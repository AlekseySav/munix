#include "kernel.h"

PUBLIC void panic(const char * msg)
{
    printk("\033[1;34mKernel panic:\033[0m %s\n", msg);
    
    ASM("cli\n"
        "hlt\n"
        ".word 0xfeeb");    // loop forever
}
