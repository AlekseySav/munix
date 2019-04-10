#ifndef _STDDEF_H_
#define _STDDEF_H_

#ifndef U_INT
#define U_INT
typedef unsigned long u_int;
#endif

#ifndef SIZE_T
#define SIZE_T
typedef u_int size_t;
#endif

#ifndef NULL
    #define NULL ((void *)0)
#endif

#endif
