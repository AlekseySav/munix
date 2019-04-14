AS = gcc -m32 -traditional -c

CC = gcc -m32
C_INC = -nostdinc -I ../include/
CFLAGS = -c $(C_INC) -Wall -fno-stack-protector	# it's bad thing not to check stack, but have to...

LD = ld -m elf_i386 -r

OBJECTS = console.o vsprintf.o

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

all: kernel.o

kernel.o: $(OBJECTS)
	$(LD) -o $@ $^

console.o: console.c

vsprintf.o: vsprintf.c

clean:
	rm -f *.o
