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
