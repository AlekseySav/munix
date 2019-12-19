include ../Makefile.inc

INCLUDE += ../include

OBJECTS = \
	alarm.o \
	ctype.o \
	errno.o \
	fork.o \
	getpid.o \
	memcpy.o \
	pause.o \
	strerror.o \
	strlen.o \
	vsprintf.o \
	write.o

lib.a: $(OBJECTS)
	$(AR) rsc $@ $^

clean:
	rm -f lib.a $(OBJECTS)

#
# dependences
#
alarm.o: alarm.c

ctype.o: ctype.c

errno.o: errno.c

fork.o: fork.c

getpid.o: getpid.c

memcpy.o: memcpy.c

pause.o: pause.c

strerror.o: strerror.c

strlen.o: strlen.c

vsprintf.o: vsprintf.c

write.o: write.c
