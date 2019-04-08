#include <stdlib.h>     // for exit
#include <stdio.h>      // for fprintf
#include <string.h>     // for strcmp

#include <unistd.h>     // for read/write
#include <fcntl.h>      // for open

#define AOUT_HEADER 32
#define ELF_HEADER 0x1000
#define MAGIC 0x04100301

void die(char * str)
{
	fprintf(stderr, "%s\n", str);
	exit(1);
}

void usage(void)
{
	die("Usage: build boot systems");
}

void boot(char * file)
{
    char buf[512];

    int fd = open(file, O_RDONLY);
    if(fd < 0) die("E: Can't open boot file");

    if(read(fd, buf, AOUT_HEADER) != AOUT_HEADER)
        die("E: Can't read boot header");
    if(((long *)buf)[0] != MAGIC)
        die("E: Non-Minix header in boot file");
    if(((long *)buf)[1] != AOUT_HEADER)
        die("E: Non-Minix header in boot file");
    if(((long *)buf)[3] != 0)
        die("E: Invalid data section in boot file");
    if(((long *)buf)[4] != 0)
        die("E: Invalid bss section in boot file");
    if(((long *)buf)[5] != 0)
        die("E: Non-Minix header in boot file");
    if(((long *)buf)[7] != 0)
        die("E: Invalid symbol table in boot file");
    
    for(int i = 0; i < 510; i++) buf[i] = 0;
    
    if(read(fd, buf, sizeof(buf)) > 510)
        die("E: size of boot can't exceed 510 bytes");
    close(fd);

    buf[510] = 0x55;
    buf[511] = 0xaa;

    if(write(1, buf, 512) != 512)
        die("E: Failed write boot data");
}

void kernel(char * file)
{
    char buf[0x1000];
    int fd;

	if ((fd = open(file, O_RDONLY, 0)) < 0)
		die("E: Unable to open kernel file");
	if (read(fd, buf, ELF_HEADER) != ELF_HEADER)
		die("E: Unable to read header of kernel");

    int c;
	for (int i = 0 ; (c = read(fd, buf, sizeof buf)) > 0 ; i += c )
		if (write(1, buf, c) != c)
			die("Write call failed");
	close(fd);
}

int main(int argc, char ** argv)
{
    if(argc != 3) usage();

    boot(argv[1]);
    kernel(argv[2]);

    return(0);
}
