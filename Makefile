AS86 = as86 -0 -a
LD86 = ld86 -0

AS = gcc -m32 -traditional -c

CC = gcc -m32
C_INC = -nostdinc -I include/
CFLAGS = -c $(C_INC)

LD = ld -s -x -m elf_i386 -Ttext 0x0000

%.o: %.s
	$(AS) -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

all: munix.img

munix.img: boot/boot system tools/build
	./tools/build boot/boot system > $@

boot/boot: boot/boot.s
	$(AS86) -o $@.o $<
	$(LD86) -0 -s -o $@ $@.o

system: boot/head.o init/main.o kernel/kernel.o
	$(LD) $^ -o $@
	objcopy -O binary -R .note  -R .comment -S $@ $@

boot/head.o: boot/head.s

init/main.o: init/main.c

kernel/kernel.o:
	(cd kernel; make)

tools/build: tools/build.c
	$(CC) -o $@ $<

clean:
	rm -f boot/*.o boot/head boot/boot
	rm -f tools/build
	rm -f init/*.o
	rm -f system
	(cd kernel; make clean)

test: munix.img
	qemu-system-i386 -full-screen -fda $<
	make clean
	rm munix.img

hex: munix.img
	hexdump -C $<
	make clean
	rm munix.img
