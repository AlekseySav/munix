include ../Makefile.inc

INCLUDE += ../include

OBJECTS = ctype.o errno.o fork.o memcpy.o strerror.o strlen.o vsprintf.o

lib.a: $(OBJECTS)
	$(AR) rsc $@ $^

clean:
	rm -f lib.a $(OBJECTS)

#
# dependences
#
ctype.o: ctype.c

errno.o: errno.c

fork.o: fork.c

memcpy.o: memcpy.c

strerror.o: strerror.c

strlen.o: strlen.c

vsprintf.o: vsprintf.c
