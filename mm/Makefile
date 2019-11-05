AS = gcc -m32 -traditional -c

CC = gcc -m32
C_INC = -nostdinc -I ../include/
CFLAGS = -c $(C_INC) -fno-stack-protector

LD = ld -m elf_i386 -r

OBJECTS = memory.o

%.o: %.s
	$(AS) -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

mm.o: $(OBJECTS)
	$(LD) $^ -o $@

memory.o: memory.c

clean:
	rm -f $(OBJECTS) mm.o
