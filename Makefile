BOOT = nasm -f bin
ASM = nasm -f elf32
CC = gcc -m32 -c -nostdinc -I include/

LD = ld -m elf_i386 -e start

TEST = qemu-system-i386 -full-screen -fda
HEX = hexdump -C 

%.o: %.asm
	$(ASM) -o $@ $<
%.o: %.c
	$(CC) -o $@ $<
%.bin: %.asm
	$(BOOT) -o $@ $<

all: munix.img

munix.img: boot/boot.bin init.bin
	cat $^ > $@

init.bin: boot/head.o init/main.o
	$(LD) $^ -o init.img
	objcopy -O binary -R .note -R .comment -S init.img $@

boot/boot.bin: boot/boot.asm

boot/head.o: boot/head.asm

init/main.o: init/main.c

test:
	make
	$(TEST) munix.img
	make clean

hex:
	make
	$(HEX) munix.img
	make clean

clean:
	rm boot/*.bin
	rm boot/*.o
	rm init/*.o
	rm *.bin
	rm *.img
