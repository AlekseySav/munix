#ifndef ASM_SEGMENT_H_
#define ASM_SEGMENT_H_

INLINE unsigned char get_fs_byte(const char * ptr)
{
    unsigned register char res;
    ASM("movb %%fs:%1, %0" : "=r" (res) : "m" (*ptr));
    return res;
}

#endif
