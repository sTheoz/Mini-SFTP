#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 
#include "gene.h"

#define MAX 64;

void diffie(void) 
{ 
    char buff[MAX]; 
    int n;

	uint64 secret = recup(sockfd);

	uint64 secret2 = recup(sockfd);

	uint32_t key[4];

	key[0]=secret&0xffff;
	key[1]=secret>>0xffff;
	key[3]=secret2&0xffff;
	key[4]=secret2>>0xffff;

} 