#ifndef _UNISTD_H_
#define _UNISTD_H_

int alarm(long seconds);
int fork(void);
int getpid(void);
int getppid(void);
int pause(void);
int write(int fd, char * buf, int nr);

#endif
