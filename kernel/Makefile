AS = gcc -m32 -traditional -c

CC = gcc -m32
C_INC = -nostdinc -I ../include/
CFLAGS = -c $(C_INC) -fno-builtin -fno-stack-protector

LD = ld -m elf_i386 -r

OBJECTS = \
	asm.o \
	clock.o \
	console.o \
	lib.o \
	panic.o \
	printk.o \
	sched.o \
	sys.o \
	syscall.o \
	trap.o \
	tty.o

%.o: %.s
	$(AS) -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

all: kernel.o

kernel.o: $(OBJECTS)
	$(LD) $^ -o $@

asm.o: asm.s

clock.o: clock.c

console.o: console.c

lib.o: lib.c

panic.o: panic.c

printk.o: printk.c

sched.o: sched.c

sys.o: sys.c

syscall.o: syscall.s

trap.o: trap.c

tty.o: tty.c

clean:
	rm -f *.o
