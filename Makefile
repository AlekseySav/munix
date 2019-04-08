AS86 = as86 -0 -a
LD86 = ld86 -0

AS = gcc -m32 -traditional -c
CC = gcc -m32
CFLAFS = -nostdinc -I include/
LD = ld -s -x -m elf_i386

%.o: %.s
	$(AS) -o $@ $<

all: munix.img

munix.img: boot/boot kernel tools/build
	./tools/build boot/boot kernel > $@

boot/boot: boot/boot.s
	$(AS86) -o $@.o $<
	$(LD86) -0 -s -o $@ $@.o

kernel: boot/head.o
	$(LD) $^ -o $@

boot/head.o: boot/head.s

tools/build: tools/build.c
	$(CC) -o $@ $<

clean:
	rm -f boot/*.o boot/head boot/boot
	rm -f tools/build
	rm -f kernel

test: munix.img
	qemu-system-i386 -full-screen -fda $<
	make clean
	rm munix.img

hex: munix.img
	hexdump -C $<
	make clean
	rm munix.img
