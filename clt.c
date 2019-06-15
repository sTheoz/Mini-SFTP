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
    if(argc<4){
        printf("Il faut 3 arguments : %s  <adresse du srv> <numéro de port> <commande> <options... \n",argv[0]);
        return 0;
    }
    char buf[BFSZ];
    struct sockaddr_in scksrv,sckclt;
    pid_t pid;
    int sfd,l,ll,n=0,fd,octet=0,nwsfd,m,length, error;
    int n_request;
    sfd = socket(AF_INET, SOCK_STREAM,0);
    //Préparation d'une struct sockaddr_in avec les coordonnées du serveur
    l=sizeof(struct sockaddr_in);
    memset(&scksrv,0,l);
    scksrv.sin_family = AF_INET;
    inet_pton(AF_INET,argv[1],&scksrv.sin_addr);
    scksrv.sin_port=htons((unsigned short)strtol(argv[2],NULL,0));
    //Demande de connexion
    error = connect(sfd,(struct sockaddr *)&scksrv,l);
    if(error == -1 ){
        perror("Connection failed ...");
        exit(1);
    }

    //Echange des clés avec Diffie

    //Construction de la requête (put, get ou dir)
    n_request = f_request(argv[3]);

    if(n_request == REQUEST_PUT && argc => 4){
        r_put(argv[4]);
    }else
    if(n_request == REQUEST_GET && argc => 4){
        r_get(argv[4]);
    }else
    if(n_request == REQUEST_DIR && argc => 4){
        r_dir(argv[4]);
    }else
    if(n_request == REQUEST_ERROR){
        perror("Problème de nom de la requête");
        exit(2);
    }else{
        perror("Pas assez d'arguments pour cette commande");
        exit(3);
    }


    //Lit la reponse du serveur

    //Vérifie la réponse ??

    //Echange les données 
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