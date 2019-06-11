#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BFSZ 256

int main(int argc, char const *argv[])
{
    if(argc!=4){
        printf("Il faut 3 arguments : %s  <adresse du srv> <numéro de port> <Nom du fichier>\n",argv[0]);
        return 0;
    }
    char buf[BFSZ];
    struct sockaddr_in scksrv,sckclt;
    pid_t pid;
    int sfd,l,ll,n=0,fd,octet=0,nwsfd,m,length;
    sfd = socket(AF_INET, SOCK_STREAM,0);
    //Préparation d'une struct sockaddr_in avec les coordonnées du serveur
    l=sizeof(struct sockaddr_in);
    memset(&scksrv,0,l);
    scksrv.sin_family = AF_INET;
    inet_pton(AF_INET,argv[1],&scksrv.sin_addr);
    scksrv.sin_port=htons((unsigned short)strtol(argv[2],NULL,0));
    connect(sfd,(struct sockaddr *)&scksrv,l);
    //Il entre dans une boucle d'envoie
    fd=open(argv[3],O_RDWR|O_CREAT);
    while(1){
        length = read(fd,buf,256);
        m=send(sfd,buf,length,0);
        //n=recv(sfd,buf,BFSZ,0);
        if(m == 0)break;
        printf("Octets envoyés : %d\n",m);
        fflush(stdout);
    }
    close(nwsfd);
    return 0;
}