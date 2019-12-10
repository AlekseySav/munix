include ../Makefile.inc

INCLUDE += ../include

OBJECTS = ctype.o memcpy.o strlen.o vsprintf.o

lib.a: $(OBJECTS)
	$(AR) rsc $@ $^

clean:
	rm -f lib.a $(OBJECTS)

#
# dependences
#
ctype.o: ctype.c

memcpy.o: memcpy.c

strlen.o: strlen.c

vsprintf.o: vsprintf.c
