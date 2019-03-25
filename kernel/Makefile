ASM = nasm -f elf32
CC = gcc -m32 -c -nostdinc -I ../include/
LD = ld -m elf_i386

OBJECTS = main.o console.o interrupts.o

%.o: %.asm
	$(ASM) -o $@ $<

%.o: %.c
	$(CC) -o $@ $<

all: kernel.o

kernel.o: $(OBJECTS)
	$(LD) -r -o $@ $^

console.o: console.c

interrupts.o: interrupts.c

init/main.o: init/main.c

clean:
	rm -f *.o
