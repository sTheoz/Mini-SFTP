#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>


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

void decrypt(uint32_t* v, uint32_t* k){
    uint32_t v0=v[0], v1=v[1], sum=0, i;          
    uint32_t delta=0x9e3779b9;                     
    uint32_t k0=k[0], k1=k[1], k2=k[2], k3=k[3];
    sum = delta * 32;
    for (i=0; i < 32; i++) {                       
        v1 -= ((v0<<4) + k2) ^ (v0 + sum) ^ ((v0>>5) + k3);
        v0 -= ((v1<<4) + k0) ^ (v1 + sum) ^ ((v1>>5) + k1);
        sum -= delta;
    }                                              
    v[0]=v0; v[1]=v1;
}
/*
int main(int argc, char** argv){
    if(argc != 5){
        printf("Usage : %s <-d ou -e> <filekey> <file1> <file2>",argv[0]);
        return 1;
    }
    uint32_t v[2];
    uint32_t k[4];

    int fd1 = open(argv[2], O_RDONLY,0600);
    if (fd1 <= 0) {
        perror("open 1");
        exit(1);
    }

    read(fd1,k,sizeof(uint32_t)*4);

    int fd, length, fd2;
    fd2 = open(argv[4], O_RDWR|O_CREAT, 0775);
    if(strcmp(argv[1], "-e") == 0){
    	fd = open(argv[3], O_RDWR|O_CREAT);
        while( length = read(fd,v,sizeof(uint32_t)*2) == 8){
                encrypt(v,k);
                write(fd2,v,sizeof(uint32_t)*2);
            }
    }else{
    	if(strcmp(argv[1], "-d")== 0){
            fd = open(argv[3], O_RDWR|O_CREAT);
            while( length = read(fd,v,sizeof(uint32_t)*2 == 8)){
                decrypt(v,k);
                write(fd2,v,sizeof(uint32_t)*2);
            }
        }
    }
    printf("Avant : v0 = %d, v1 = %d\n", v[0],v[1]);
    encrypt(v,k);
    printf("Chiffrement : v0 = %d, v1 = %d\n", v[0],v[1]);
    decrypt(v,k);
    printf("Dechiffrement : v0 = %d, v1 = %d\n", v[0],v[1]);
    return 0;
}*/