#ifndef _ASM_SYSTEM_H_
#define _ASM_SYSTEM_H_

#ifndef cli
    #define cli() ASM("cli")
#endif

#ifndef sti
    #define sti() ASM("sti")
#endif

#endif
