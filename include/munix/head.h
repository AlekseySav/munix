#ifndef _MUNIX_HEAD_H_
#define _MUNIX_HEAD_H_

struct table_struct {
    uint32_t a, b;
};

EXTERN struct table_struct idt[256];
EXTERN struct table_struct gdt[256];

EXTERN uint32_t pg_dir[1024];

#endif
