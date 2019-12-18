/*
 * mm/memory.c
 * 
 * defines functions for memory handling
 */

#include <munix/config.h>
#include <munix/kernel.h>
#include <munix/mm.h>

#define PAGING_MEMORY   (HIGH_MEMORY - LOW_MEMORY)
#define PAGING_PAGES    (PAGING_MEMORY / PAGE_SIZE)

static char mem_map[PAGING_PAGES] = { 0 };

long get_free_page(void)
{
    int res;
    asm("cld\n\t"
        "repne; scasb\n\t"          // find free page
        "jne 1f\n\t"
        "decl %%edi\n\t"
        "movl $1, (%%edi)\n\t"      // mark page as non-free
        "subl $mem_map, %%edi\n\t"
        "shll $12, %%edi\n\t"
        "addl %1, %%edi\n\t"
        "movl %%edi, %0\n\t"
        "xorl %%eax, %%eax\n\t"
        "movl $1024, %%ecx\n\t"
        "rep; stosl\n\t"            // zero page
        "1:"
        :   "=m" (res)
        :   "i" (LOW_MEMORY),
            "a" (0), "c" (PAGING_PAGES), "D" (mem_map)
        );
    return res;
}

void free_page(long addr)
{
    register long page = addr;
    page -= LOW_MEMORY;
    page >>= 12;

    if(page >= PAGING_PAGES)
        panic("Trying to free unexisted page");
    if(--mem_map[page])
        panic("Trying to free free page");
}


// small util, that display memory info
void sys_setup(void)
{
    unsigned i, used;

    printk("Paging memory:\t%4d KiB = %4d pages\n", HIGH_MEMORY / 1024, HIGH_MEMORY / PAGE_SIZE);
    printk("Kernel size:\t%4d KiB = %4d pages\n", LOW_MEMORY / 1024, LOW_MEMORY / PAGE_SIZE);
    
    used = 0;
    for(i = 0; i < PAGING_PAGES; i++)
        if(mem_map[i]) used++;

    printk("Used memory:\t%4d KiB = %4d pages\n", used * 4, used);
    printk("Free memory:\t%d KiB = %d pages\n", ((HIGH_MEMORY - LOW_MEMORY) / 1024 - (used * 4)),
        ((HIGH_MEMORY - LOW_MEMORY) / PAGE_SIZE - used));
}
