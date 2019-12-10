LD		= ld
CC		= gcc
AS		= gcc	# gcc automaticly compiles .S files using assembler
				# if not, change this to "as" or smth like this

LDFLAG	= -s -x -m elf_i386
ASFLAG	= -c -m32 -nostdinc -I $(INCLUDE)
CFLAGS	= -c -m32 -nostdinc -I $(INCLUDE) \
    -fno-builtin -fno-stack-protector \
    -fstrength-reduce -fomit-frame-pointer

AR		= ar

%: %.c			# c utils
	$(CC) -o $@ $<

%.o: %.c		# kernel c files
	$(CC) $(CFLAGS) -o $@ $<

%.o: %.S		# gas files
	$(AS) $(ASFLAG) -o $@ $<
