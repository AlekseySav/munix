#ifndef _SYS_CONST_H_
#define _SYS_CONST_H_

#ifndef EXTERN
    #ifdef _LIBRARY
        #define EXTERN
    #else
        #define EXTERN extern
    #endif
#endif

#ifndef INLINE
    #ifdef _LIBRARY
        #define INLINE
    #else
        #define INLINE inline
    #endif
#endif

#ifndef CONST
    #define CONST const
#endif

#ifndef STATIC
    #define STATIC static
#endif

#ifndef PRIVATE
    #define PRIVATE STATIC
#endif

#ifndef PUBLIC
    #define PUBLIC
#endif

#ifndef ASM
    #define ASM asm
#endif

#ifndef VOLATILE
    #define VOLATILE volatile
#endif

#ifndef TRUE
    #define TRUE 1
#endif

#ifndef FALSE
    #define FALSE 0
#endif

#endif
