AS = gcc -m32 -traditional -c

CC = gcc -m32
C_INC = -nostdinc -I ../include/
CFLAGS = -c $(C_INC)

LD = ld -m elf_i386 -r

OBJECTS = asm.o console.o lib.o printk.o tty.o

%.o: %.s
	$(AS) -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

all: kernel.o

kernel.o: $(OBJECTS)
	$(LD) $^ -o $@

asm.o: asm.s

console.o: console.c

lib.o: lib.c

printk.o: printk.c

tty.o: tty.c

clean:
	rm -f *.o
