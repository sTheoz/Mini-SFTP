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
#include "fileF.h"
#include "gene.h"

#define BFSZ 256

int sendToServ(int fd , void* buf, int size){
    int n;
    n = send(fd , buf , size, 0);
    return n;
}

struct answer readServ(int fd){
    ssize_t n;
    struct answer ans;
    recv(fd, &ans , sizeof(ans), 0);
    if( n == -1 ){
        perror("Erreur lors de la reception de la reponse");
        exit(5);
    }
    return ans;
}


int main(int argc, char const *argv[])
{
    if(argc<4){
        printf("Il faut 3 arguments : %s  <adresse du srv> <commande> <options... \n",argv[0]);
        return 0;
    }
    char buf[BFSZ];
    struct sockaddr_in scksrv;
    int sfd,l,n=0,fd,nwsfd,m,length, error;
    int n_request=0;
    uint64 result_A;

    struct request r;
    struct answer ans;


    sfd = socket(AF_INET, SOCK_STREAM,0);
    //Préparation d'une struct sockaddr_in avec les coordonnées du serveur
    l=sizeof(struct sockaddr_in);
    memset(&scksrv,0,l);
    scksrv.sin_family = AF_INET;
    inet_pton(AF_INET,argv[1],&scksrv.sin_addr);
    scksrv.sin_port=htons(7716);
    //Demande de connexion
    error = connect(sfd,(struct sockaddr *)&scksrv,l);
    if(error == -1 ){
        perror("Connection failed ...");
        exit(1);
    }
    printf("Connexion établie avec le serveur...%d\n", n_request);
    //Echange des clés avec Diffie

    result_A = sendPrimeNumber(sfd);

    //Construction de la requête (put, get ou dir)
    n_request = num_request(argv[2]);
    printf("Numero de requete %d\n",n_request);

    if(n_request == REQUEST_PUT && argc >= 5){
        r = r_put(argv[3]);
    }else
    if(n_request == REQUEST_GET && argc >= 5){
        r = r_get(argv[3]);
    }else
    if(n_request == REQUEST_DIR && argc >= 4){
        r = r_dir(argv[3]);
    }else
    if(n_request == REQUEST_ERROR){
        perror("Problème de nom de la requête");
        exit(2);
    }else{
        perror("Pas assez d'arguments pour cette commande");
        exit(3);
    }

    n = sendToServ(sfd, &r, sizeof(r));
    if( n == -1 ){
        perror("Erreur lors de l'envoie de la requête");
        exit(4);
    }

    //Lit la reponse du serveur

    ans = readServ(sfd);
    if(ans.ack == ANSWER_UNKNOWN){
        perror("Requete inconnue...");
        exit(5);
    }else if( ans.ack == ANSWER_ERROR){
        perror("Erreur reponse du serveur");
        exit(6);
    }else if( ans.ack == ANSWER_OK ){
        if(n_request == REQUEST_GET){
            int nbytes = ans.nbbytes;
            printf("Je vais recup %d\n",nbytes);
            recvFile(argv[4], sfd, ans.nbbytes, result_A); //r.path = serverpath
        }else if(n_request == REQUEST_PUT){
            printf("Envoie du fichier en cours...\n");
            sendFile(argv[4], sfd, r.nbbytes,result_A);
        }else if(n_request == REQUEST_DIR){
            recvDir(sfd);
        }
    }else{
        perror("Erreur reponse du serveur incohérente");
        exit(7);
    }
    //Vérifie la réponse ??

    //Echange les données 
    /*while(1){
        length = read(fd,buf,256);
        m=send(sfd,buf,length,0);
        //n=recv(sfd,buf,BFSZ,0);
        if(m == 0)break;
        printf("Octets envoyés : %d\n",m);
        fflush(stdout);
    }
    close(nwsfd);*/
    close(sfd);
    return 0;
}

