#include <stdlib.h>     // for exit
#include <stdio.h>      // for fprintf
#include <string.h>     // for strcmp

#include <unistd.h>     // for read/write
#include <fcntl.h>      // for open

#define MINIX_HEADER 32
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

    if(read(fd, buf, MINIX_HEADER) != MINIX_HEADER)
        die("E: Can't read boot header");
    if(((long *)buf)[0] != MAGIC)
        die("E: Non-Minix header in boot file");
    if(((long *)buf)[1] != MINIX_HEADER)
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

    fd = open(file, O_WRONLY);
    if(write(fd, buf, 512) != 512)
        die("E: Failed write boot data");
    close(fd);
}

void apply(char * flag, char * arg)
{
    if(strcmp(flag, "--boot") == 0)
        boot(arg);
}

int main(int argc, char ** argv)
{
    if(argc < 3) usage();

    for(int i = 1; i < argc; i += 2)
        apply(argv[i], argv[i + 1]);
}
