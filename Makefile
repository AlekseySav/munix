BOOT = nasm -f bin
ASM = nasm -f elf32
CC = gcc -m32 -c
LD = ld -m elf_i386 -Ttext 0x0060 -e start
TEST = qemu-system-i386 -full-screen -fda

all:
	$(BOOT) -o boot.bin boot/boot.asm

	$(ASM) -o head.o boot/head.asm
	# $(CC) -o main.o init/main.c

	$(LD) *.o -o head.out
	objcopy -O binary -R .note -R .comment -S head.out head.bin

	cat boot.bin head.bin > munix.img

	make test
	make clean

test:
	$(TEST) munix.img

clean:
	rm *.o
	rm *.bin
	rm *.out
