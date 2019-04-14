CC = gcc -m32
C_INC = -nostdinc -I ../include/
CFLAGS = -c $(C_INC)

AR = ar
OBJS = string.o

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

all: lib.a

lib.a: $(OBJS)
	$(AR) rsc $@ $(OBJS)

string.o: string.c


clean:
	rm -f *.o
	rm -f *.a
