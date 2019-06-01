#define _CRT_SECURE_NO_WARNINGS

#ifdef __APPLE__
    #include <stdlib.h>
    #include <malloc/malloc.h>
#else
    #include <stdio.h>
    #include <malloc.h>
#endif

#include <cstring>
#include <time.h>

#define MAX_SYL	4
#define SH		8

typedef unsigned int	uint32_t;
typedef unsigned long long	uint64_t;

time_t timer = time( &timer );
unsigned int seed = (int) timer;
unsigned int dees = ( seed >> SH ) | ( seed << (32 - SH) );
unsigned long long randomNum = (seed >> 16) | (seed << 16);
							//	0	  1		2	   3	  4				6			  8				10			  12		 14			 16		
const char *	startSyl[]	= {"Mi", "Ni",  "A", "Ba",  "Bo",  "Re",  "Di", "Kon",   "Fi",  "Va", "Vla", "Pri", "Na", "Ko", "De", "Il", "Li", "E"};
const char *	midSyl[]	= {"mi", "ni", "na",  "a", "ron", "don", "kon", "fon",  "lo", "s\'k", "rom"};
const char *	endSyl[]	= {"im", "mi", "ni", "er", "der",  "un",  "or",  "ok",    "n",   "f",   "s",  "al",  "l",  "m", "ko",  "a"};



uint32_t lcg_rand(uint32_t a) {
    return ((uint64_t)a * 279470273UL) % 4294967291UL;
}


unsigned long nextRandom(unsigned int range = 1) {
	randomNum = lcg_rand(randomNum);
	return randomNum % range;
}

char * GenerateName() {
	unsigned int sylCount = nextRandom(MAX_SYL);
	sylCount += 2;
	unsigned char startNum = nextRandom( 16 );
	char* result = (char*) malloc( 3 * sylCount + 1 );
	strcpy( result, (char*) startSyl[ startNum ] );
	for ( unsigned int i = 0; i < sylCount - 2; i++ ) {
		strcat( result, midSyl[ nextRandom( 10 ) ] );
	}
	strcat( result, endSyl[nextRandom( 15 )] );
	return (char*) result;
}