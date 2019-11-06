#ifndef _SYS_TYPES_H_
#define _SYS_TYPES_H_

// it's for 32-bit system

typedef signed char int8_t;
typedef unsigned char uint8_t;

typedef signed short int16_t;
typedef unsigned short uint16_t;

typedef signed int int32_t;
typedef unsigned int uint32_t;

// for termios
typedef unsigned short tcflag_t;
typedef unsigned char cc_t;

// for stackframe_s
typedef short seg_t;
typedef long reg_t;

// for system calls
typedef int (*fn_ptr)(void);

#endif
