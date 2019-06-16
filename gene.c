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

//Deux fonctions majoritairement pris sur Lucas N.
int checkPrimeNumber(int num)
{
    if (num <= 1)
    {
        return 0;
    }

    // Even numbers.
    if (num % 2 == 0)
    {
        return num == 2; // 2 is the only even prime.
    }

    // Odd numbers.
    // Start loop at 3 and step 2 to skip even divisors.
    for (int i = 3; i * i <= num; i += 2)
    {
        if (num % i == 0)
        {
            return 0;
        }
    }
    return 1;
}


uint64 sendPrimeNumber(int socket)
{
	uint64 result_A;
    srand(time(NULL));
    int number = rand();
    while (checkPrimeNumber(number) != 1){
        number = rand();
    }
	result_A = envoi(socket,number);
    return result_A;
}