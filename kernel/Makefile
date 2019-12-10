include ../Makefile.inc

INCLUDE += ../include

OBJECTS = asm.o console.o fork.o panic.o printk.o sched.o timer.o trap.o tty.o

kernel.o: $(OBJECTS)
	$(LD) $(LDFLAG) -r -o $@ $^

clean:
	rm -f kernel.o $(OBJECTS)

#
# dependences
#
asm.o: asm.S

console.o: console.c

fork.o: fork.c

panic.o: panic.c

printk.o: printk.c

sched.o: sched.c

timer.o: timer.S

trap.o: trap.c

tty.o: tty.c
