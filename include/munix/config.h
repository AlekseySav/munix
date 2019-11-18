#if !defined(_ANSI) || _ANSI < 31459
    #error "Need _ANSI must be declared for correct config.h work"
#endif

#ifndef _MUNIX_CONFIG_H_
#define _MUNIX_CONFIG_H_

#define NR_TASKS 64

#define LOW_MEM     0x100000
#define HIGH_MEM    0x800000        // 8 MiB

#endif
