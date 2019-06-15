#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>

typedef unsigned __int128 uint128;
typedef unsigned long long int uint64;

uint64 expm(uint64 m, uint64 e, uint64 n){
	uint128 _r=1U;
	uint128  _m=(uint128)m;
	uint128 _mod = (uint128)n;
	while (e){
		if (e & 1) _r=(_r*_m)%_mod;
		_m=(_m*_m)%_mod;
		e>>=1;
	}
	return (uint64)_r;
	//calcule m^e modulo n
}

uint64 generateur(uint64 p){
	uint64 nb_e = p-1;
	uint64 mid = nb_e/2;
	uint64 i=2;
	while(expm(i,mid,p) != 1){
		i++;
	}
	return i;
	//calcule un generateur de (Zp)*
	//en suppposant que p est un nombre premier de Sophie Germain
}

uint64 recup(int sockfd){
	uint64 p;
	uint64 g;
	uint64 b = (rand() %(p-2))+ 2;
	uint64 ga;
	uint64 gb;

	read(sockfd, &p, sizeof(p));
	read(sockfd, &g, sizeof(g));
	read(sockfd, &ga, sizeof(ga));

	gb = expm(g,b,p);

	write(sockfd, &gb, sizeof(gb));

	return expm(ga,b,p);
}

uint64 envoi(int sockfd, uint64 sophie){
	uint64 p = sophie;
	uint64 g = generateur(p);
	uint64 a = (rand() %(p-2))+ 2;
	uint64 ga = expm(g,a,p);
	uint64 gb;

	write(sockfd, &p, sizeof(p));
	write(sockfd, &g, sizeof(g));
	write(sockfd, &ga, sizeof(ga));

	read(sockfd, &gb, sizeof(gb));

	return expm(gb,a,p);
}