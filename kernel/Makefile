AS = gcc -m32 -traditional -c

CC = gcc -m32
C_INC = -nostdinc -I ../include/
CFLAGS = -c $(C_INC)

LD = ld -m elf_i386 -r

OBJECTS = console.o

%.o: %.s
	$(AS) -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

all: kernel.o

kernel.o: $(OBJECTS)
	$(LD) $^ -o $@

console.o: console.c

clean:
	rm -f *.o
