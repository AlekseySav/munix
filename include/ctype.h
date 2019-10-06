#ifndef _CTYPE_H_
#define _CTYPE_H_

EXTERN unsigned char _ctype[];

#define _U	0x01	/* upper */
#define _L	0x02	/* lower */
#define _D	0x04	/* digit */
#define _C	0x08	/* cntrl */
#define _P	0x10	/* punct */
#define _S	0x20	/* white space */
#define _X	0x40	/* hex digit */
#define _SP	0x80	/* hard space (0x20) */

#define isupper(c)  ((_ctype + 1)[(c)] & (_U))
#define islower(c)  ((_ctype + 1)[(c)] & (_L))
#define isdigit(c)  ((_ctype + 1)[(c)] & (_D))
#define iscntrl(c)  ((_ctype + 1)[(c)] & (_C))
#define ispunct(c)  ((_ctype + 1)[(c)] & (_P))
#define isspace(c)  ((_ctype + 1)[(c)] & (_S))

#define isalpha(c)  ((_ctype + 1)[(c)] & (_U | _L))
#define isalnum(c)  ((_ctype + 1)[(c)] & (_U | _L | _D))
#define isgraph(c)  ((_ctype + 1)[(c)] & (_U | _L | _D | _P))
#define isprint(c)  ((_ctype + 1)[(c)] & (_U | _L | _D | _P | _SP))
#define isxdigit(c) ((_ctype + 1)[(c)] & (_D | _X))

#define isascii(c)  ((_ctype + 1)[(c)])
#define toascii(c)  ((c) & 0x7f)

#define toupper(c)  (islower(c) ? ((c) + ('A' - 'a')) : (c))
#define tolower(c)  (isupper(c) ? ((c) + ('a' - 'A')) : (c))

#endif
