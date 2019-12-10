/*
 * head.S declares varibles, defined in boot/head.S
 */

struct table_entry
{
    long a, b;
};

extern struct table_entry gdt[256];
extern struct table_entry idt[256];

extern char pg_dir[4096];
