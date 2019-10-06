#ifndef _STDARG_H_
#define _STDARG_H_

#ifndef VA_LIST
#define VA_LIST va_list
typedef char * va_list;
#endif

#define __va_rounded(type) \
    (((sizeof(type) + sizeof(int) - 1) / sizeof(int)) * sizeof(int))

#define va_start(ap, last) \
    ap = (((char *)&last) + __va_rounded(last))

#define va_end(ap)

#define va_arg(ap, type) \
    ((ap += __va_rounded(type)), \
    (*(type *)(ap - __va_rounded(type))))

#endif
