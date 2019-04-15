#include <sys/const.h>
#include <stdarg.h>     // for va_list, va_arg
#include <stddef.h>     // for NULL, for string.h
#include <string.h>     // for strcpy, strlen

PRIVATE const char * big = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
PRIVATE const char * small = "0123456789abcdefghijklmnopqrstuvwxyz";

// n = n / base && return n % base;
#define div(n, base) ({ \
    unsigned _res = (unsigned)n % (unsigned)base; \
    n /= (unsigned)base; \
    _res; })

#define is_digit(x) (x >= '0' && x <= '9')

#define ZEROPAD	1		// pad with zero
#define SIGN	2		// unsigned/signed long
#define PLUS	4		// show plus
#define SPACE	8		// space if plus
#define LEFT	16		// left justified
#define SPECIAL	32		// 0x, etc.
#define SMALL	64		// use 'abcdef...' instead of 'ABCDEF...'

PRIVATE int skip_atoi(const char ** ptr) 
{
    int res = 0;
    while is_digit(**ptr) {
        res = res * 10 + *((*ptr)++) - '0';
    }
    return res;
}

PRIVATE char * numstr(char * buf, int num, int base, int size, int precision, int flagss)
{
	if (base < 2 || base > 36) return NULL;

    char sign = 0, space, tmp[36];  // no need in more than 36 chars
    const char * digits;
    int i = 0;

    if(flagss & SMALL) digits = small;
    else digits = big;
    space = (flagss & ZEROPAD) ? '0' : ' ';  // fill free space

    if(flagss & SIGN) {
        if(num < 0) {
            sign = '-';
            num = -num;
        }
        else if(flagss & PLUS) sign = '+';
        else if(flagss & SPACE) sign = ' ';
    }

    if(sign) size--;    // size if free space

    if(num == 0) tmp[i++] = '0';
    else while(num != 0)
        tmp[i++] = *(digits + div(num, base));
    if(i > precision) precision = i;
    size -= precision;

    if(!(flagss & (LEFT | ZEROPAD)))
        while(size-- > 0)
            *buf++ = space;

    if(sign)
        *buf++ = sign;
    if(flagss & SPECIAL) {
        if(base == 8) {
            *buf++ = '0';
            size--;
        }
        else if(base == 16) {
            *buf++ = '0';
            *buf++ = 'x';
            size -= 2;
        }
    }

    if(!(flagss & LEFT)) {
        while(size-- > 0)
            *buf++ = space;
    }
	while(i < precision--)
		*buf++ = '0';
    while(i-- > 0)
        *buf++ = tmp[i];
    while(size-- > 0)
        *buf++ = ' ';
    return buf;
}

PUBLIC int vsprintf(char * buf, const char * fmt, va_list args)
{
    char * str = buf;
    while(*fmt) {
        if(*fmt != '%')
            *str++ = *fmt++;
        else {

            // get flagss
            int flags = 0;
            while(*fmt++) {
                if(*fmt == '0')
                        flags |= ZEROPAD;
                else if(*fmt == '+')
                        flags |= PLUS;
                else if(*fmt == ' ')
                        flags |= SPACE;
                else if(*fmt == '-')
                        flags |= LEFT;
                else if(*fmt == '#')
                        flags |= SPECIAL;
                else break;
            }

            // get field width
            int field_width = -1;
            if is_digit(*fmt)   // if number - it is field width
                field_width = skip_atoi(&fmt);
            else if(*fmt == '*') { // if star - it's next arg
                field_width = va_arg(args, int);
                if(field_width < 0) {
                    field_width = -field_width;
                    flags |= LEFT;
                }
            }

            // get precision
            int precision = -1;
            if(*fmt == '.') {
                fmt++;
                if is_digit(*fmt)
                    precision = skip_atoi(&fmt);
                else if(*fmt == '*')
                    precision = va_arg(args, int);
                if(precision < 0) precision = 0;
            }

            char * s;   // for 's'
            size_t len; // for 's'
            int * ip;   //for 'n'

            switch(*fmt++) {  // now it is type of print
                case 'c':                           // write char
                    if(!(flags & LEFT))
                        while(field_width-- > 1)    // because one symbol is char
                            *str++ = ' ';
                    *str++ = va_arg(args, char);
                    while(field_width-- > 1)
                        *str++ = ' ';
                    break;
                case 's':                           // write string
                    s = va_arg(args, char *);
                    len = strlen(s);
                    if (precision < 0)
                        precision = len;
                    else if (len > precision)
                        len = precision;
                        
                    field_width -= len;
                    if(!(flags & LEFT))
                        while(field_width-- > 0)
                            *str++ = ' ';
                    while(len-- > 0)
                        *str++ = *s++;
                    while(field_width-- > 0)
                        *str++ = ' ';
                    break;
                case 'o':                           // write oct-number
                    str = numstr(str, va_arg(args, unsigned), 8, field_width, precision, flags);
                    break;
                case 'p':                           // write pointer
                    str = numstr(str, (unsigned)va_arg(args, void *), 16, field_width, precision, flags);
                    break;
                case 'x':                           // little digits (16)
                    flags |= SMALL;
                case 'X':                           // big digits (16)
                    str = numstr(str, va_arg(args, int), 16, field_width, precision, flags);
                    break;
                case 'd':                           // signed number (10)
                case 'i':                           // same
                    flags |= SIGN;
                case 'u':                           // unsigned
                    str = numstr(str, va_arg(args, int), 10, field_width, precision, flags);
                break;
                case 'n':                           // written symbols
                    ip = va_arg(args, int *);
                    *ip = (str - buf);
                    break;
                default:                            // else '%' is default symbol
                    *str++ = '%';
                    if(*fmt != '%' && *fmt != '\0')
                        *str++ = *fmt;
                    else if(*fmt == '%')
                        fmt--;
                    break;
            }
        }
    }
    *str = '\0';
    va_end(args);   // there's no nessesary to write it, but why not :-)
    return str - buf;
}
