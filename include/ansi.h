#ifndef _ANSI_H_
#define _ANSI_H_

#ifndef __GNUC__
    #error I want gcc!
#endif

#ifndef PACKED
    #define PACKED __attribute((packed))
#endif

#endif
