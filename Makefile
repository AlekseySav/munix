include Makefile.inc

INCLUDE = include
LDFLAG += -T link.ld

usage:
	@echo "Usage:" >&2
	@echo "  make Image                build munix image" >&2
	@echo "  make world                make all utilities etc." >&2
	@echo "  make clean                remove all temporary files" >&2

Image: world
	tools/build.sh boot/boot system > $@

world: boot/boot system
	chmod +x tools/*.sh

clean:
	rm -f boot/boot system
	rm -f boot/*.o init/*.o
	tools/build.sh $@
	(cd kernel; make $@)
	(cd lib; make $@)

__test:
	make clean
	make Image
	qemu-system-i386 -full-screen -fda Image
	hexdump -C Image
	make clean
	rm Image

boot/boot: boot/boot.o
	$(LD) $(LDFLAG) -o $@ $^

system: boot/head.o init/main.o kernel/kernel.o lib/lib.a
	$(LD) $(LDFLAG) -o $@ $^

boot/boot.o: boot/boot.S

boot/head.o: boot/head.S

init/main.o: init/main.c

kernel/kernel.o:
	(cd kernel; make)

lib/lib.a:
	(cd lib; make)
