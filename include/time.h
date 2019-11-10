#ifndef _TIME_H_
#define _TIME_H_

#include <sys/types.h>

struct tm {
    int tm_sec;
    int tm_min;
    int tm_hour;
    int tm_mday;
    int tm_mon;
    int tm_year;
    int tm_wday;
    int tm_yday;
    int tm_isdst;
};

char * asctime(const struct tm * tm);
time_t mktime(struct tm * tm);

#endif
