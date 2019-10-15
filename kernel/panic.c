#include "kernel.h"

PUBLIC void panic(const char * msg)
{
    printk("Kernel panic: %s\n", msg);
    
    ASM("cli\n"
        "hlt\n"
        ".word 0xfeeb");    // loop forever
}
