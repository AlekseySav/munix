#ifndef _MUNIX_MM_H_
#define _MUNIX_MM_H_

#define PAGE_SIZE 4096

unsigned long get_free_page(void);
void free_page(unsigned long addr);

#endif
