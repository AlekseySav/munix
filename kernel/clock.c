#include "kernel.h"

#define RTC_READ(addr) ({ \
    outb_p(0x80 | addr, 0x70); \
    inb_p(0x71); \
    })

#define IS_UPDATING() ({\
    outb_p(0x0a, 0x70); \
    inb_p(0x71) & 0x80; \
    })

#define BCD_TO_BIN(x) ((x) = (((x) & 0xf) + (((x) >> 4) * 10)))

PRIVATE INLINE void read_tm(struct tm * tm)
{
    tm->tm_isdst = 0;
    tm->tm_sec = RTC_READ(0);
    tm->tm_min = RTC_READ(2);
    tm->tm_hour = RTC_READ(4);
    tm->tm_wday = RTC_READ(6);
    tm->tm_mday = RTC_READ(7);
    tm->tm_mon = RTC_READ(8) - 1;
    tm->tm_year = RTC_READ(9);
    
    /* it's not a year day, it's current century
     * mb ugly, but i have to save it somewhere
     */
    tm->tm_yday = RTC_READ(0x32);
}

PRIVATE INLINE void to_bin(struct tm * tm)
{
    BCD_TO_BIN(tm->tm_sec);
    BCD_TO_BIN(tm->tm_min);
    BCD_TO_BIN(tm->tm_hour);
    BCD_TO_BIN(tm->tm_wday);
    BCD_TO_BIN(tm->tm_mday);
    BCD_TO_BIN(tm->tm_mon);
    BCD_TO_BIN(tm->tm_year);
    BCD_TO_BIN(tm->tm_yday);        // it is still century
}

PUBLIC time_t startup_time;

PUBLIC void clock_init(void)
{
    int type;
    struct tm tm;
    struct tm prev;

    while(IS_UPDATING());

    read_tm(&prev);

    do {
        tm = prev;
        read_tm(&prev);
    } while(memcmp(&prev, &tm, sizeof(struct tm)));

    type = RTC_READ(0x0b);

    // convert bcd to binary
    if(!(type & 0x04))
        to_bin(&tm);
    
    // convert 12-hour format to 24-hour
    if(!(type & 0x02) && (tm.tm_hour & 0x80))
        tm.tm_hour = (tm.tm_hour & 0x7f + 12) % 24;

    // add century and normalize everything
    tm.tm_year += (tm.tm_yday - 19) * 100;
    tm.tm_yday = -1;
    tm.tm_isdst = -1;

    startup_time = mktime(&tm);
    printk("%s", asctime(&tm));
}
