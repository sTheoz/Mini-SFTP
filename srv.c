#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "request.h"
#include "response.h"
#include "tea.h"
#include "gene.h"

#define BFSZ 256

int main(void)
{
    char buf[BFSZ];
    struct sockaddr_in scksrv,sckclt;
    pid_t pid;
    int sfd,l,ll,n=0,fd,nwsfd, error;

    struct request r;
    uint64 result_A;
    
    sfd = socket(AF_INET, SOCK_STREAM,0);
    if(sfd == -1){
        perror("Socket creation");
        exit(1);
    }
    //Préparation d'une struct sockaddr_in avec les coordonnées du serveur
    l=sizeof(struct sockaddr_in);
    memset(&scksrv,0,l);
    scksrv.sin_family = AF_INET;
    scksrv.sin_addr.s_addr = INADDR_ANY;
    scksrv.sin_port = htons(7716);
    error = bind(sfd,(struct sockaddr*) &scksrv , l);
    if(error == -1 ){
        perror("Binding");
        exit(2);
    }
    listen(sfd,0);


    //Il entre dans une boucle d'extraction des demandes de connexions
    //fd=open(file,O_RDWR|O_CREAT,0600);
    bzero(buf,BFSZ);
    while(1){
        ll=l;
        printf("En attente de connexion...\n");
        //Accepte une nouvelle connexion
        nwsfd = accept(sfd,(struct sockaddr *)&sckclt , &ll);
        //nwsfd est un nouveau descripteur de socket pr à être utilisé pour déployer avec ce nouveau client
        pid=fork();
        //Rentre dans la partie du fils
        if(pid == 0){
            close(sfd);
            while(1){
                //Echange de clé avec diffie

                result_A = recup(nwsfd);

                printf("J'ai %lld\n", result_A);

                //Lit la requête
                r = readRequest(nwsfd);
                //La requête est dans la structure
                //Identifie et contruction de la reponse
                n = sendResponse(nwsfd, r,result_A);
                //Envoie de la réponse

                //L'échange de donnée se fait avec TEA

                /* bzero(buf,BFSZ);
                n=recv(nwsfd,buf,BFSZ,0);
                write(fd,buf,256);
                if(n==0){
                    close(nwsfd);
                    exit(0);
                }
                printf("Octects reçu : %d\n",n);*/
                //Réponse au client
                //m=send(nwsfd,buf,BFSZ,0);
            }
        }
        printf("Connexion réalisé avec le client %d\n",nwsfd);
        //Ferme la socket du nouveau client dans le père afin d'attendre une nouvelle connexion à nouveau
        close(nwsfd);
        
    }
    return 0;
}
