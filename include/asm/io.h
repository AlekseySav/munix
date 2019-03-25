#ifndef _ASM_IO_H_
#define _ASM_IO_H_

#ifndef outb
    #define outb(value, port) asm("outb %%al,%%dx"::"a" (value),"d" (port))
#endif

#ifndef outb_p
    #define outb_p(value,port) \
                    asm("outb %%al,%%dx\n\t" \
		                "jmp 1f\n\t" \
		                "1:jmp 1f\n\t" \
		                "1:"::"a" (value),"d" (port))
#endif
#endif
