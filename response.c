#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>

#include "fileF.h"
#include "request.h"

#define ANSWER_OK 0
#define ANSWER_UNKNOWN 1      /* requête inconnue */
#define ANSWER_ERROR 2        /* erreur lors du traitement */

struct answer {
	int  ack;
	int  nbbytes;  /* pour GET seulement */
	int  errnum;   /* significatif ssi != 0 et ack == ANSWER_ERROR */
	int _pad[1];   /* aligne la taille sur un multiple de 8 octets */
};


int sendResponse(int fd, struct request r, uint64 cle){
	struct answer a;
	int file,n;
	unsigned int size;
	if(r.kind == REQUEST_GET){
		//tester le path
		size = sizeFile(r.path);
		if(size == -1){
			a.ack = ANSWER_ERROR;
			a.errnum = 3; // 3 = fichier introuvable 
		}else{
			a.ack = ANSWER_OK;
			a.errnum = 0;
			a.nbbytes = size;
		}
	}else if(r.kind == REQUEST_PUT){
		if(r.nbbytes > 0 && r.path != NULL){
			a.ack = ANSWER_OK;
			a.errnum = 0;
		}else{
			a.ack = ANSWER_ERROR;
			a.errnum = 1; // 1 = erreur	de la commande
		}
	}else if(r.kind == REQUEST_DIR){
		a.ack = ANSWER_OK;
		a.errnum = 0;
	}else{
		a.ack = ANSWER_UNKNOWN;
		a.errnum = 2; // 2 = inconnue
	}
	//Envoie de la reponse
	n = send(fd, &a, sizeof(a), 0);
	if(n == -1){
		perror("Erreur lors de l'envoie de la reponse");
		exit(2);
	}

	if(r.kind == REQUEST_DIR)sendDir(r.path, fd);
	if(r.kind == REQUEST_GET)sendFile(r.path, fd, a.nbbytes, cle);
	if(r.kind == REQUEST_PUT)recvFile(r.path, fd, r.nbbytes, cle);
	return 0;
}