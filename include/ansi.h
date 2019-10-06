#ifndef _ASNI_H_
#define _ANSI_H_

#ifndef _ANSI
    #if defined(__STDC__) || defined(__GNUC__)
        #define _ANSI 31459
    #endif
#endif

#if !defined(_ANSI) || _ANSI < 31459
    #error "Need ansi-compatable c/c++ compiler"
#endif

#define ATTRIBUTE(x) __attribute__((x))
#define PACKED ATTRIBUTE(__packed__)

#endif
