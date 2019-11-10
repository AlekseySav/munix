#include <time.h>

#define MIN 60
#define HOUR (MIN * 60)
#define DAY (HOUR * 24)
#define YEAR (DAY * 365)

static int month[12] = {
    DAY * (0),
    DAY * (31),
    DAY * (31 + 29),
    DAY * (31 + 29 + 31),
    DAY * (31 + 29 + 31 + 30),
    DAY * (31 + 29 + 31 + 30 + 31),
    DAY * (31 + 29 + 31 + 30 + 31 + 30),
    DAY * (31 + 29 + 31 + 30 + 31 + 30 + 31),
    DAY * (31 + 29 + 31 + 30 + 31 + 30 + 31 + 31),
    DAY * (31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30),
    DAY * (31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31),
    DAY * (31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30),
};

static const char * cdays[] = {
    "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
};

static const char * cmonths[] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun", 
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

static char ascii[26];

extern int sprintf(char * buf, const char * fmt, ...);

char * asctime(const struct tm * tm)
{
    sprintf(ascii, "%s %s %d %d:%d:%d %d\n", 
        cdays[tm->tm_wday],
        cmonths[tm->tm_mon],
        tm->tm_mday,
        tm->tm_hour,
        tm->tm_min,
        tm->tm_sec,
        1900 + tm->tm_year);
    return ascii;
}

time_t mktime(struct tm * tm)
{
    time_t res;
    int year;

    year = tm->tm_year - 70;

    res = YEAR * year;
    res += DAY * ((year + 1) / 4);
    
    res += month[tm->tm_mon];
    if(tm->tm_mon >= 2 && ((year + 2) % 4) != 0)
        res -= DAY;

    res += DAY * tm->tm_mday;
    res += HOUR * tm->tm_hour;
    res += MIN * tm->tm_min;
    res += tm->tm_sec;

    return res;
}
