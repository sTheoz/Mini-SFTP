#include <stdint.h>

typedef unsigned long long int uint64;

unsigned int sizeFile(char path[]);

int sendDir(char file[], int fd);

int sendFile(char file[], int fd, int size, uint64 cle);

int recvFile(char file[],int fd, int size, uint64 cle);
int recvDir(int fd);