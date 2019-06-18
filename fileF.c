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

unsigned int sizeFile(char* path){
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

int sendDir(char* file, int fd){
    close(1);
    stdout = dup(fd);
    execl("/bin/ls","ls","-l",file, NULL);
    return 0;
}

int sendFile(char* file, int fd, int size, uint64 cle){
    int fd_file,n,m,total=0;
    u_int32_t v[2];
    u_int32_t k[4]={0};
    *k=cle;

    *(k+2)=cle;

    fd_file = open(file,O_RDONLY);
    while( total <= (size-8) ){
        n = read(fd_file,v, sizeof(uint32_t)*2);
        if(n == -  1){
            perror("Erreur sur le read");
        }
        encrypt(v,k);
        m = send(fd, v, sizeof(uint32_t)*2,0);
        total += m;
        //printf("Envoie des données : %d otects\n", total);
        if(m == -1){
            perror("Erreur lors de l'envoie du fichier apres chiffrement");
        }
    }
    n = read(fd_file,v, size-total);
    if(n == -  1){
        perror("Erreur sur le read");
    }
    encrypt(v,k);
    m = send(fd, v, size-total,0);
    total += m;
    printf("Envoie des données : %d otects\n", total);
    if(m == -1){
        perror("Erreur lors de l'envoie du fichier apres chiffrement");
        return -1;
    }
    //Savoir combien de zero a rajouter pour remplir le paquet
    /*memset(v,0,sizeof(uint32_t)*2);
    */

    return 0;
}

int recvFile(char* file,int fd, int size, uint64 cle){
    int fd_file,n,m,total=0;
    u_int32_t v[2];
    u_int32_t k[4]={0};
    *k=cle;

    *(k+2)=cle;

    fd_file = open(file,O_RDWR|O_CREAT,0640);
    while(  total <= (size-8) ){
        n = read(fd,v, sizeof(uint32_t)*2);
        if(n == -  1){
            perror("Erreur sur le read");
        }
        decrypt(v,k);
        m = write(fd_file, v, sizeof(uint32_t)*2);
        total += m;
        if(m == -1){
            perror("Erreur de reception du fichier lors de l'écriture");
        }
    }
    n = read(fd,v, size-total);
    if(n == -  1){
        perror("Erreur sur le read");
    }
    decrypt(v,k);
    m = write(fd_file, v, size-total);
    //Il faudrait supprimer les derniers octets en trop du fichiers
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
    int m;
    stdout = dup(1);
    while( read(fd,buf,8) ){
        m = write(stdout, buf, 8);
        if(m == -1){
            perror("Erreur de reception du fichier lors de l'écriture");
        }
    }
    if(m == -1){
        perror("Erreur de reception du fichier lors de l'écriture");
        return -1;
    }
    return 0;
}