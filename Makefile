all:
	nasm -f bin -o boot.bin boot/boot.asm
	nasm -f bin -o head.bin boot/head.asm

	cat boot.bin head.bin > munix.img
	qemu-system-i386 -full-screen -fda munix.img
