CC = gcc
C_INC = -nostdinc -I ../include/
CFLAGS = -m32 -c $(C_INC) -fno-builtin -fno-stack-protector

AR = ar
OBJS = \
	ctype.o \
	sprintf.o \
	string.o \
	time.o \
	vsprintf.o

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

all: lib.a

lib.a: $(OBJS)
	$(AR) rsc $@ $(OBJS)

ctype.o: ctype.c

sprintf.o: sprintf.c

string.o: string.c

time.o: time.c

vsprintf.o: vsprintf.c

clean:
	rm -f *.o
	rm -f *.a
