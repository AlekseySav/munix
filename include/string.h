#ifndef _STRING_H_
#define _STRING_H_

#include <stddef.h>

const char * strerror(int err);

size_t strlen(const char * str);
void * memcpy(void * dest, void * src, size_t len);

#endif
