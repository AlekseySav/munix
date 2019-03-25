ASM = nasm -f elf32
CC = gcc -m32 -c -nostdinc -I ../ -I ../include/
LD = ld -m elf_i386

OBJECTS = console.o

%.o: %.asm
	$(ASM) -o $@ $<

%.o: %.c
	$(CC) -o $@ $<

all: kernel.o

kernel.o: $(OBJECTS)
	$(LD) -r -o $@ $^

console.o: console.c

clean:
	rm *.o
