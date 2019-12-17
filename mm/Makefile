include ../Makefile.inc

INCLUDE += ../include

OBJECTS = memory.o

mm.o: $(OBJECTS)
	$(LD) $(LDFLAG) -r -o $@ $^

clean:
	rm -f mm.o $(OBJECTS)

#
# dependences
#
memory.o: memory.c
