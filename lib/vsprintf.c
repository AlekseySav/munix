#include <stdarg.h>
#include <string.h>
#include <ctype.h>

// flags for vsprintf

#define ZEROPAD	    0x0001		// pad with zero
#define SIGN	    0x0002		// unsigned/signed long
#define PLUS	    0x0004		// show plus
#define SPACE	    0x0008		// space if plus
#define LEFT	    0x0010		// left justified
#define SPECIAL	    0x0020		// 0x, etc.
#define SMALL	    0x0040		// use 'abcdef...' instead of 'ABCDEF...'

// n = n / base && return n % base;
#define div(n, base) ({ \
    int res; \
    asm("divl %4" \
        : "=a" (n), "=d" (res) \
        : "a" (n), "d" (0), "r" (base)); \
    res;  })

static int skip_atoi(const char ** ptr)
{
    int res = 0;
    while(isdigit(**ptr)) {
        res = res * 10 + *((*ptr)++) - '0';
    }
    return res;
}

static char * print_number(char * buf, int number, unsigned base, int size, int precision, int flags)
{
    static const char * small = "0123456789abcdef";
    static const char * big = "0123456789ABCDEF";

    const char * digits;
    char tmp[36];       // no need in more than 36 symbols
    char sign = '\0';
    int special;
    int i = 0;

    if(flags & SMALL)
        digits = small;
    else 
        digits = big;

    if(flags & SIGN) {
        size--;
        if(number < 0) {
            number = -number;
            sign = '-';
        }
        else if(flags & SPACE) sign = ' ';
        else if(flags & PLUS) sign = '+';
        else size++;
    }

    do {
        tmp[i++] = digits[div(number, base)];
    } while(number);

    while(i < precision)
        tmp[i++] = '0';

    special = i;
    if(flags & SPECIAL) {
        if(base == 16) {
            if(flags & SMALL)
                tmp[special++] = 'x';
            else
                tmp[special++] = 'X';
        }
        if(base == 8 || base == 16)
            tmp[special++] = '0';
    }
    size -= special;
    
    if(!(flags & (ZEROPAD | LEFT)))
        while(size-- > 0)
            *buf++ = ' ';

    if(sign) *buf++ = sign;

    while(special-- > i)
        *buf++ = tmp[special];

    if((!(flags & LEFT)))
        while(size-- > 0)
            *buf++ = '0';
    
    while(i--)
        *buf++ = tmp[i];

    if(flags & LEFT)
        while(size-- > 0)
            *buf++ = ' ';

    return buf;
}

static char * print_null(char * buf, int size)
{
    const char * null = "(null)";

    size -= 6;
    while(size-- > 0)
        *buf++ = ' ';
    while(*null)
        *buf++ = *null++;
    return buf;
}

static inline char * print_pointer(char * buf, void * ptr, int size, int precision,int flags)
{
    if(ptr == NULL)
        return print_null(buf, size);
    return print_number(buf, (int)ptr, 16, size, precision, flags);
}

static inline char * print_string(char * buf, char * str, int size, int precision, int flags)
{
    if(str == NULL)
        return print_null(buf, size);

    int len = strlen(str);

    if(precision && len > precision)
        len = precision;

    size -= len;

    while(size-- > 0)
        *buf++ = ' ';

    while(len-- > 0)
        *buf++ = *str++;
    return buf;
}

static inline char * do_printf(char * buf, char type, int size, int precision, int flags, va_list * args)
{
    switch(type) {
        case 'd':                               // signed decimal int
        case 'i': flags |= SIGN;                // ...
        case 'u':                               // unsigned decimal
            buf = print_number(buf, va_arg(*args, int), 10, size, precision, flags);
            break;
        case 'o':                               // unsigned octal
            buf = print_number(buf, va_arg(*args, int), 8, size, precision, flags);
            break;
        case 'p':                               // pointer
            flags |= SMALL;
            buf = print_pointer(buf, va_arg(*args, void *), size, precision, flags);
            break;
        case 'x':                               // unsigned small hexadecimal
            flags |= SMALL;
        case 'X':                               // unsigned big hexadecimal
            buf = print_number(buf, va_arg(*args, int), 16, size, precision, flags);
            break;
        case 'c':                               // char
            *buf++ = va_arg(*args, char);
            break;
        case 's':
            buf = print_string(buf, va_arg(*args, char *), size, precision, flags);
            break;
        default:
            *buf++ = '%';                       // else, '%' is default symbol
            if(type != '%')
                *buf++ = type;
            break;
    }
    return buf;
}

int vsprintf(char * buf, const char * fmt, va_list args)
{
    char c;
    int flags;
    int size;
    int precision;
    char * b = buf;
    
    while(*fmt) {
        if(*fmt != '%') {
            *b++ = *fmt++;
            continue;
        }

        flags = 0;
        for(;;) {
            c = *++fmt;
            if(c == '#') flags |= SPECIAL;
            else if(c == '0') flags |= ZEROPAD;
            else if(c == '-') flags |= LEFT;
            else if(c == ' ') flags |= SPACE;
            else if(c == '+') flags |= PLUS;
            else break;
        }

        if(*fmt == '*') {
            size = va_arg(args, int);
            fmt++;
        }
        else
            size = skip_atoi(&fmt);
            
        if(*fmt == '.') {
            fmt++;
            if(*fmt == '*') {
                precision = va_arg(args, int);
                fmt++;
            }
            else precision = skip_atoi(&fmt);
        }
        else precision = 0;

        if(*fmt) {
            b = do_printf(b, *fmt, size, precision, flags, &args);
            fmt++;
        }
    }
    return b - buf;
}
