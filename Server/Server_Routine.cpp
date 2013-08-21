#include "Server_References.h"

int lockMutex( SDL_mutex * mutex ) {
	int r = SDL_mutexP( mutex );
	if ( r == -1 ) {
		printf( "Couldn't lock mutex\n" );
		exit( -1 );
	}
	return r;
}

int unlockMutex( SDL_mutex * mutex ) {
	int r = SDL_mutexV( mutex );
	if ( r == -1 ) {
		printf( "Couldn't unlock mutex\n" );
		exit( -1 );
	}
	return r;
}

Uint32 convFromIP( Uint8 a, Uint8 b, Uint8 c, Uint8 d ) {
	Uint8 * conv = (Uint8*) malloc( 4 );
	conv [0] = a;
	conv [1] = b;
	conv [2] = c;
	conv [3] = d;

	return *((Uint32*) conv);
}

const char * getStringIP( Uint32 ip ) {
	int a, b, c, d;

	Uint8 * dotQuad = (Uint8*)&ip;
	a = (unsigned short) dotQuad[0];
	b = (unsigned short) dotQuad[1];
	c = (unsigned short) dotQuad[2];
	d = (unsigned short) dotQuad[3];
	char *result = (char*) malloc(16);
	sprintf(result, "%i.%i.%i.%i", a, b, c, d);
	return result;
}

const char * getStringAddress( IPaddress ip ) {
	char * result = (char*) malloc(24);
	sprintf(result, "%s:%i", getStringIP(ip.host), ip.port);
	return result;
}
