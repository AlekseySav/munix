NAME = munix.img

BOOT = nasm -f bin
ASM = nasm -f elf32
CC = gcc -m32 -c -nostdinc -I / -I include/
LD = ld -m elf_i386 -e start -Ttext 0x1000
BIN = objcopy -O binary

TEST = qemu-system-i386 -full-screen -fda
HEX = hexdump -C

%.bin: %.asm
	$(BOOT) -o $@ $<

%.o: %.asm
	$(ASM) -o $@ $<

%.o: %.c
	$(CC) -o $@ $<

all: $(NAME)

$(NAME): boot/boot.bin kernel.bin
	cat $^ > $@

kernel.bin: boot/head.o kernel/kernel.o init/main.o
	$(LD) -o $@ $^
	$(BIN) -S $@ $@

boot/boot.bin: boot/boot.asm

boot/head.o: boot/head.asm

kernel/kernel.o:
	(cd kernel; make)

init/main.o: init/main.c

clean:
	rm -f kernel.bin
	(cd boot/; rm -f *.bin)
	(cd boot/; rm -f *.o)
	(cd init/; rm -f *.o)
	(cd kernel/; make clean)

test:
	make
	make clean
	$(TEST) $(NAME)
	rm $(NAME)

hex:
	make
	make clean
	$(HEX) $(NAME)
	rm $(NAME)
