#define _GNU_SOURCE 1
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/fcntl.h>
#include <stdint.h>
#include <string.h>

void encrypt (uint32_t* v, uint32_t* k) {
    uint32_t v0=v[0], v1=v[1], sum=0, i;           /* set up */
    uint32_t delta=0x9e3779b9;                     /* a key schedule constant */
    uint32_t k0=k[0], k1=k[1], k2=k[2], k3=k[3];   /* cache key */
    for (i=0; i < 32; i++) {                       /* basic cycle start */
        sum += delta;
        v0 += ((v1<<4) + k0) ^ (v1 + sum) ^ ((v1>>5) + k1);
        v1 += ((v0<<4) + k2) ^ (v0 + sum) ^ ((v0>>5) + k3);
    }                                              /* end cycle */
    v[0]=v0; v[1]=v1;
}

void decrypt (uint32_t* v, uint32_t* k) {
    uint32_t v0=v[0], v1=v[1], sum=0, i;           /* set up */
    uint32_t delta=0x9e3779b9;                     /* a key schedule constant */
    uint32_t k0=k[0], k1=k[1], k2=k[2], k3=k[3];   /* cache key */
    sum = 32*delta;
    for (i=0; i < 32; i++) {                       /* basic cycle start */
        v1 -= ((v0<<4) + k2) ^ (v0 + sum) ^ ((v0>>5) + k3);
        v0 -= ((v1<<4) + k0) ^ (v1 + sum) ^ ((v1>>5) + k1);
        sum -= delta;
    }                                              /* end cycle */
    v[0]=v0; v[1]=v1;
}

// int 4 octet

ssize_t filesize(int fd) {
	struct stat s;
	int r = fstat(fd, &s);
	if (r >= 0 && S_ISREG(s.st_mode) && s.st_size <= SSIZE_MAX) {
		return s.st_size;
	} else {
		return -1;
	}
}

int main(int argc, char const **argv){

    char which;

    if (argc !=5) {
       printf("%s <option> <key> <file1> <file2>\n",argv[0]);
       exit(-1);
    }

    if (strcmp(argv[1],"-e")==0) {
        which = 'e';
    }
    if (strcmp(argv[1],"-d")==0) {
        which = 'd';
    }

    uint32_t key[4];
    uint32_t octet[2];

    int fd1 = open(argv[2], O_RDONLY,0600);
    if (fd1 <= 0) {
        perror("open 1");
        exit(1);
    }

    read(fd1,key,sizeof(uint32_t)*4);
    
    int fd2 = open(argv[3], O_RDONLY,0600);
    if (fd2 <= 0) {
        perror("open 2");
        exit(1);
    }

    int fd3 = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC,0600);
    if (fd3 <= 0) {
        perror("open 3");
        exit(1);
    }

    if (which == 'd') {

        int actu = 0;
        int size = filesize(fd2);

         while(read(fd2,octet,sizeof(uint32_t)*2) == 8){
            actu += 8;
            decrypt(octet, key);
            if(actu == (size)){
                char *p=(char*)octet;
                char x = p[7];
                write(fd3,octet,8-x);
            } else {
                write(fd3,octet,sizeof(uint32_t)*2);
            }
        }
        
    }

    if (which == 'e') {

        int actu = 0;
        int size = filesize(fd2);
        int recup ;
         while(1){
            recup = read(fd2,octet,sizeof(uint32_t)*2);
            
            actu += recup;
            if(recup != 8){
                int rest = 8 - recup ;
                
                char* c = (char *) octet;
                c[7]=rest;

            }
            encrypt(octet, key);
            write(fd3,octet,sizeof(uint32_t)*2);
            if ((recup) < 8) {
                break;
            }
        }
        
    }
    
    return 0;
}
