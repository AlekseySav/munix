# makefile for munix
# make (or make all or make munix.img) creates the kernel file (munix.img)
# make clean delete all output files
# make test run kernel in qemu
# make hex dumps system

BOOT = nasm -f bin
AS = nasm -f elf32
LD = ld -m elf_i386 -e start -Ttext 0x0000
BIN = objcopy -O binary -R .note -R .comment -S

%.bin: %.asm
	$(BOOT) -o $@ $<

%.o: %.asm
	$(AS) -o $@ $<

all: munix.img


munix.img: boot/boot.bin kernel
	cat $^ > $@

kernel: boot/head.o
	$(LD) $^ -o $@
	$(BIN) $@ $@

boot/boot.bin: boot/boot.asm

boot/head.o: boot/head.asm

TEST = qemu-system-i386 -full-screen -fda
HEX = hexdump -C

clean:
	rm boot/boot.bin
	rm boot/head.o
	rm kernel

test:
	make all
	make clean
	$(TEST) munix.img
	rm munix.img

hex:
	make all
	make clean
	$(HEX) munix.img
	rm munix.img
