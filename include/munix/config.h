#ifndef _MUNIX_CONFIG_H_
#define _MUNIX_CONFIG_H_

#define LOW_MEMORY  0x100000        // 1MiB is space for kernel
#define HIGH_MEMORY 0x400000        // Qemu provides only 4MiB of memory

#define SYSSIZE     0x0100          // size of kernel in sectors
#define INITSEG     0x9000          // used in boot.S, console.c

#define NR_TASKS    64

#define _KERNEL

#endif
