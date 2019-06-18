#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>

#include "request.h"
#include "response.h"
#include "tea.h"
#include "gene.h"

#define BFSZ 256


//Fonction majoritairement pris sur Lucas N.
int create_tcp_server()
{
    int sfd;
    struct addrinfo hints, *result = NULL, *rp = NULL;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;     /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* TCP socket */
    hints.ai_flags = AI_PASSIVE;     /* For wildcard IP address */
    hints.ai_protocol = 0;           /* Any protocol */
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    if ((sfd = getaddrinfo(NULL,"7716", &hints, &result)) != 0)
    {
        fprintf(stderr, "\t => getaddrinfo: %s\n", gai_strerror(sfd));
        exit(EXIT_FAILURE);
    }
    if (result == NULL)
    {
        perror("\t => No adress succeeded\n");
        exit(EXIT_FAILURE);
    }
    /* Création de la socket. */
    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1)
            continue;
        if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
        {
            break;
        }
        else
        {
            perror("\t => Erreur sur bind()\n");
            close(sfd);
            sfd = -1;
        }
    }
    /* Mise en écoute de la socket. */
    if (-1 == (listen(sfd, 0)))
    {
        perror("\t => Erreur sur listen()\n");
        exit(EXIT_FAILURE);
    }
    return (sfd);
}

int main(void)
{
    char buf[BFSZ];
    struct sockaddr_in sckclt;
    pid_t pid;
    int sfd,ll,nwsfd;

    struct request r;
    uint64 result_A;
    
    /*sfd = socket(AF_INET, SOCK_STREAM,0);
    if(sfd == -1){
        perror("Socket creation");
        exit(1);
    }*/

    sfd=create_tcp_server();
    //Préparation d'une struct sockaddr_in avec les coordonnées du serveur
    /* l=sizeof(struct sockaddr_in);
    memset(&scksrv,0,l);
    scksrv.sin_family = AF_INET;
    scksrv.sin_addr.s_addr = INADDR_ANY;
    scksrv.sin_port = htons(7716);
    error = bind(sfd,(struct sockaddr*) &scksrv , l);
    if(error == -1 ){
        perror("Binding");
        exit(2);
    }
    listen(sfd,0);*/


    //Il entre dans une boucle d'extraction des demandes de connexions
    //fd=open(file,O_RDWR|O_CREAT,0600);
    bzero(buf,BFSZ);
    while(1){
        printf("En attente de connexion...\n");
        //Accepte une nouvelle connexion
        nwsfd = accept(sfd,(struct sockaddr *)&sckclt , &ll);
        signal(SIGCHLD, SIG_IGN);
        //nwsfd est un nouveau descripteur de socket pr à être utilisé pour déployer avec ce nouveau client
        pid=fork();
        //Rentre dans la partie du fils
        if(pid == 0){
            close(sfd);
            while(1){
                //Echange de clé avec diffie

                result_A = recup(nwsfd);

                //Lit la requête
                r = readRequest(nwsfd);
                //La requête est dans la structure
                //Identifie et contruction de la reponse
                sendResponse(nwsfd, r,result_A);
                //Envoie de la réponse

                //L'échange de donnée se fait avec TEA

            }
        }
        printf("Connexion réalisé avec le client %d\n",nwsfd);
        //Ferme la socket du nouveau client dans le père afin d'attendre une nouvelle connexion à nouveau
        close(nwsfd);
        
    }
    return 0;
}
