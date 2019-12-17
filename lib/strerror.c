#include <stddef.h>
#include <errno.h>

static const char * errlist[] = {
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "Resource temporarily unavailable"  /* EAGAIN */
};

const char * strerror(int err)
{
    if(err < 0 || err >= ERROR)
        return "Unknown error";
    return errlist[err];
}
