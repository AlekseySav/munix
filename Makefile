AS86 = as86 -0 -a
LD86 = ld86 -0

AS = gcc -m32 -traditional -c

CC = gcc
C_INC = -nostdinc -I include/
CFLAGS = -m32 -c $(C_INC)

LD = ld -s -x -m elf_i386 -T link.ld

%.o: %.s
	$(AS) -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

all: munix.img

munix.img: tools/build boot/boot tools/system
	tools/build boot/boot tools/system > $@

boot/boot: boot/boot.s
	$(AS86) -o $@.o $<
	$(LD86) -0 -s -o $@ $@.o

tools/system: boot/head.o init/main.o kernel/kernel.o lib/lib.a
	$(LD) $^ -o $@

boot/head.o: boot/head.s

init/main.o: init/main.c

kernel/kernel.o:
	(cd kernel; make)

lib/lib.a:
	(cd lib; make)

tools/build: tools/build.c
	$(CC) -o $@ $<

clean:
	rm -f boot/*.o boot/head boot/boot
	rm -f tools/build
	rm -f init/*.o
	rm -f tools/system
	(cd kernel; make clean)
	(cd lib; make clean)

test:
	make clean
	make
	qemu-system-i386 -full-screen -fda munix.img
	make clean
	rm munix.img

hex:
	make clean
	make
	hexdump -C munix.img
	make clean
	rm munix.img
