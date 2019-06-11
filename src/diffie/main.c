#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include "gene.h"


typedef unsigned __int128 uint128;
typedef unsigned long long int uint64;

int main(void){
	uint64 p = 4611686018427402023;
	uint64 res = generateur(p);
	printf("%d\n",res );
	return 1;
}