#define ANSWER_OK 0
#define ANSWER_UNKNOWN 1      /* requête inconnue */
#define ANSWER_ERROR 2        /* erreur lors du traitement */
typedef unsigned long long int uint64;

struct answer {
	int  ack;
	int  nbbytes;  /* pour GET seulement */
	int  errnum;   /* significatif ssi != 0 et ack == ANSWER_ERROR */
	int _pad[1];   /* aligne la taille sur un multiple de 8 octests */
};

int sendResponse(int fd, struct request r, uint64 cle);