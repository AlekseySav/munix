#include <munix/kernel.h>

const char * msg = "System loaded";
PUBLIC void main(void)
{
    int_init();
    con_init();

    con_write(msg);

    asm("int $1");  // interrupt test
}
