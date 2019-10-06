#ifndef _STDDEF_H_
#define _STDDEF_H_

#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned long long size_t;
#endif

#ifndef _PTRDIFF_T
#define _PTRDIFF_T
typedef signed long long ptrdiff_t;
#endif

#ifndef NULL
    #define NULL ((void *)0)
#endif

#endif
