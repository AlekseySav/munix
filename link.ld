ENTRY(start)
OUTPUT_ARCH(i386)
OUTPUT_FORMAT(elf32-i386)
SECTIONS
{
    . = 0;
    .text : { *(.text) }
    .data : { *(.data) }
    .bss  : { 
    __bss_start = .;
        *(.bss)
    __bss_end = .;
    }
}
