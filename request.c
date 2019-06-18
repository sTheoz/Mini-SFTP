#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#define REQUEST_PUT 1
#define REQUEST_GET 2
#define REQUEST_DIR 3
#define REQUEST_ERROR -1
#define MAXPATH 64

struct request {
    int  kind;
    char path[MAXPATH];
    int  nbbytes;        /* pour PUT seulement */
};
/* on alignera la taille de la strucuture sur un multiple de 8 octets (pour tea) en controlant
 * MAXPATH */

int num_request(char* req){
    if( strcmp(req,"put") == 0)return REQUEST_PUT;
    if( strcmp( req , "get") == 0)return REQUEST_GET;
    if( strcmp( req , "dir") == 0)return REQUEST_DIR;
    return REQUEST_ERROR;
}

struct request r_put(char* file){
    struct request r;
    r.kind = REQUEST_PUT;
    strcpy(r.path, file);
    r.nbbytes = (int) strlen(file);
    return r;
}

struct request r_get(char* file){
    struct request r;
    r.kind = REQUEST_GET;
    strcpy(r.path, file);
    return r;
}

struct request r_dir(char* directory){
    struct request r;
    r.kind = REQUEST_DIR;
    strcpy(r.path, directory);
    return r;
}

struct request readRequest(int fd){
    struct request r;
    recv(fd, &r , sizeof(r), 0);
    return r;
}
