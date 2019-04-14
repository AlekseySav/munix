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
    #ifndef LIB_SRC
        #define EXTERN extern
    #else
        #define EXTERN
    #endif
#endif

#ifndef TRUE
    #define TRUE 1
#endif

#ifndef FALSE
    #define FALSE 0
#endif

#ifndef ASM
    #define ASM asm
#endif

#endif
