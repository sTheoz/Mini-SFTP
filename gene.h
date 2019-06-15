#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>

typedef unsigned __int128 uint128;
typedef unsigned long long int uint64;

uint64 expm(uint64 m, uint64 e, uint64 n);

uint64 generateur(uint64 p);

uint64 recup(int sockfd);

uint64 envoi(int sockfd, uint64 sophie);