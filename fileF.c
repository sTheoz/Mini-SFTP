#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

#include <math.h>
#include <stdint.h>


#include "tea.h"

typedef unsigned long long int uint64;

unsigned int sizeFile(char path[]){
    FILE *file;
    unsigned int size;
 
    file=fopen(path,"r");
 
    if(file)
    {
        fseek (file, 0, SEEK_END);
        size= (unsigned int) ftell (file);
        fclose (file);          
    }else{
        perror("Erreur avec la taile du fichier");
        return -1;
    }
    return size;
}

int sendDir(char file[], int fd){
    close(1);
    stdout = dup(fd);
    execl("/bin/ls","ls","-l",file, NULL);
    return 0;
}

int sendFile(char file[], int fd, int size, uint64 cle){
    int fd_file,n,m,total=0;
    int res_temp;
    u_int32_t v[2];
    u_int32_t k[4]={0};
    *k=cle;
    *(k+1)=cle;
    *(k+2)=cle;
    *(k+3)=cle;
    //genKey(&k);
    fd_file = open(file,O_RDONLY);
    while( n = read(fd_file,v, sizeof(uint32_t)*2) == 8 ){
        encrypt(v,k);
        m = send(fd, v, sizeof(uint32_t)*2,0);
        total += m;
        //printf("Envoie des données : %d otects\n", total);
        if(m == -1){
            perror("Erreur lors de l'envoie du fichier apres chiffrement");
        }
    }
    printf("Je suis là %d \n",n);
    encrypt(v,k);
    m = send(fd, v, n,0);
    total += m;
    printf("Envoie des données : %d otects\n", total);
    if(m == -1){
        perror("Erreur lors de l'envoie du fichier apres chiffrement");
        return -1;
    }
    //Savoir combien de zero a rajouter pour remplir le paquet
    /*memset(v,0,sizeof(uint32_t)*2);
    */
    res_temp = 8 - n;

    return 0;
}

int recvFile(char file[],int fd, int size, uint64 cle){
    int fd_file,n,m,total=0;
    int res_temp;
    u_int32_t v[2];
    u_int32_t k[4]={0};
    *k=cle;
    *(k+1)=cle;
    *(k+2)=cle;
    *(k+3)=cle;
    //genKey(&k);
    fd_file = open(file,O_RDWR|O_CREAT,0640);
    //fd_file = open("test.txt",O_RDWR|O_CREAT,0777);
    while( n = read(fd,v, sizeof(uint32_t)*2) == sizeof(uint32_t)*2 ){
        decrypt(v,k);
        m = write(fd_file, v, sizeof(uint32_t)*2);
        total += m;
        //printf("Ecriture du fichier : %d otects\n", total );
        if(m == -1){
            perror("Erreur de reception du fichier lors de l'écriture");
        }
    }
    decrypt(v,k);
    m = write(fd_file, v, sizeof(uint32_t)*2);
    total += m;
    printf("Ecriture du fichier : %d octets\n", total);
    fflush(stdout);
    if(m == -1){
        perror("Erreur de reception du fichier lors de l'écriture");
        return -1;
    }
    return 0;
}

int recvDir(int fd){
    char buf[8];
    int n,m,sfd;
    //sfd = open("ls.data", O_RDWR|O_CREAT,0740);
    stdout = dup(1);
    while( n = read(fd,buf,8) ){
        m = write(stdout, buf, 8);
        if(m == -1){
            perror("Erreur de reception du fichier lors de l'écriture");
        }
    }
    //m = write(stdout, buf, 8);
    if(m == -1){
        perror("Erreur de reception du fichier lors de l'écriture");
        return -1;
    }
    return 0;
}