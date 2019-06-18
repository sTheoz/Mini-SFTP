#define REQUEST_PUT 1
#define REQUEST_GET 2
#define REQUEST_DIR 3
#define REQUEST_ERROR -1
#define MAXPATH 64

struct request {
    int  kind;          /* Type de requete */
    char path[MAXPATH]; 
    int  nbbytes;        /* pour PUT seulement */
};
/* on alignera la taille de la strucuture sur un multiple de 8 octets (pour tea) en controlant
 * MAXPATH */

int num_request(char* req);

struct request r_put(char* file);
struct request r_get(char* file);

struct request r_dir(char* directory);

struct request readRequest(int fd);