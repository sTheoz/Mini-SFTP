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
    if(argc!=2){
        printf("Il faut 1 argument : %s  <numéro de port>\n",argv[0]);
        return 0;
    }
    char buf[BFSZ];
    struct sockaddr_in scksrv,sckclt;
    pid_t pid;
    int sfd,l,ll,n=0,fd,octet=0,nwsfd,m;
    char file[20]="filename-0.txt";
    sfd = socket(AF_INET, SOCK_STREAM,0);
    //Préparation d'une struct sockaddr_in avec les coordonnées du serveur
    l=sizeof(struct sockaddr_in);
    memset(&scksrv,0,l);
    scksrv.sin_family = AF_INET;
    scksrv.sin_addr.s_addr = INADDR_ANY;
    scksrv.sin_port = htons((unsigned short)strtol(argv[1],NULL,0));
    bind(sfd,(struct sockaddr*) &scksrv , l);
    listen(sfd,0);
    //Il entre dans une boucle d'extraction des demandes de connexions
    fd=open(file,O_RDWR|O_CREAT,0600);
    bzero(buf,BFSZ);
    while(1){
        ll=l;
        nwsfd = accept(sfd,(struct sockaddr *)&sckclt , &ll);
        //nwsfd est un nouveau descripteur de socket pr à être utilisé pour déployer avec ce nouveau client
        pid=fork();
        if(pid == 0){
            close(sfd);
            while(1){
                bzero(buf,BFSZ);
                n=recv(nwsfd,buf,BFSZ,0);
                write(fd,buf,256);
                if(n==0){
                    close(nwsfd);
                    exit(0);
                }
                printf("Octects reçu : %d\n",n);
                //Réponse au client
                //m=send(nwsfd,buf,BFSZ,0);
            }
        }
        close(nwsfd);
    }
    return 0;
}
