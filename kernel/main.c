#include <ansi.h>
#include <munix/kernel.h>

const char * msg = "System loaded\nLog as root user...\n\nroot> ";

PUBLIC void main(void)
{
    int_init();
    con_init();

    con_write(msg);
    while(1);
}
