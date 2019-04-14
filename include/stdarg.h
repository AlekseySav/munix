#ifndef _STDARG_H_
#define _STDARG_H_

#ifndef VA_LIST
#define VA_LIST va_list
typedef char * va_list;
#endif

#undef __va_rounded
#define __va_rounded(type) \
    ((sizeof(type) + sizeof(int) - 1) / sizeof(int) * sizeof(int))

#ifndef va_start
    #define va_start(ap, last_arg) \
        (ap = (char *)&last_arg + __va_rounded(last_arg))
#endif

#ifndef va_end
    #define va_end(ap)
#endif

#ifndef va_arg
    #define va_arg(ap, type) \
        (ap += __va_rounded(type), \
        *((type *)(ap - __va_rounded(type))))
#endif

#endif
