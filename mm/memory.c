#include "../kernel/kernel.h"

PRIVATE unsigned short mem_map[PAGES] = { 0 };

PUBLIC unsigned long get_free_page(void)
{
    unsigned long page = PAGES;

    ASM("cld\n\t"
        "rep\n\t"
        "scasw\n\t"                     // find free page
        "jne 1f\n\t"
        "movl $1, %%edi\n\t"            // mark page as non-free
        "movl %%edi, %0\n\t"
        "1:"
        : "=r" (page)
        : "D" (mem_map), "c" (PAGES), "a" (0));

    if(page >= PAGES)
        return 0;
    return (page << 12) + LOW_MEM;
}

PUBLIC void free_page(unsigned long addr)
{
    if(addr < LOW_MEM)
        return;

    addr -= LOW_MEM;
    addr >>= 12;

    if(addr >= PAGES)
        panic("trying to free unexisted page");
    if(mem_map[addr]--)
        panic("trying to free free page");
}
