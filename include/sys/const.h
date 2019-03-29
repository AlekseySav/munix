#ifndef _SYS_CONST_H_
#define _SYS_CONST_H_

#ifndef STATIC
    #define STATIC static
#endif

#ifndef PRIVATE
    #define PRIVATE STATIC
#endif

#ifndef PUBLIC
    #define PUBLIC
#endif

#ifndef EXTERN
    #define EXTERN extern
#endif

#ifndef TRUE
    #define TRUE 1
#endif

#ifndef FALSE
    #define FALSE 0
#endif

#ifndef NULL
    #define NULL ((void *)0)
#endif

#ifndef ASM
    #define ASM asm                     // it is sometimes __asm or __asm__ etc.
#endif

#ifndef VOLATILE
    #define VOLATILE volatile
#endif
 
#endif
