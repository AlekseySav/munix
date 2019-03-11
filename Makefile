BOOT = nasm -f bin
ASM = nasm -f elf32
CC = gcc -m32 -c
LD = ld -m elf_i386 -Ttext 0x0060 -e start
TEST = qemu-system-i386 -full-screen -fda

all: munix.img

munix.img: boot.bin init.bin
	cat boot.bin init.bin > munix.img
	
boot.bin: boot/boot.asm
	$(BOOT) -o boot.bin boot/boot.asm

init.bin: head.o init.o
	$(LD) *.o -o head.img
	objcopy -O binary -R .note -R .comment -S head.img init.bin

head.o: boot/head.asm
	$(ASM) -o head.o boot/head.asm

init.o: init/main.c
	$(CC) -o main.o init/main.c

test:
	make
	$(TEST) munix.img
	make clean

clean:
	rm *.o
	rm *.bin
	rm *.img
