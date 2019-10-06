#ifndef _STRING_H_
#define _STRING_H_

#ifndef _SIZE_T_
#define _SIZE_T_
#include <stddef.h>
#endif

size_t strlen(const char * str);
char * strcpy(char * dest, const char * src);

#endif
