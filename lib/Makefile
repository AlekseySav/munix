CC = gcc
C_INC = -nostdinc -I ../include/
CFLAGS = -m32 -c $(C_INC)

AR = ar
OBJS = ctype.o string.o vsprintf.o

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

all: lib.a

lib.a: $(OBJS)
	$(AR) rsc $@ $(OBJS)

ctype.o: ctype.c

string.o: string.c

vsprintf.o: vsprintf.c

clean:
	rm -f *.o
	rm -f *.a
