#define _CRT_SECURE_NO_WARNINGS
#include "Client_references.h"


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

Uint32 convFromIP( int a, int b, int c, int d ) {
	return a + (b << 8) + (c << 16) + (d << 24);
}

const char * getStringIP( Uint32 ip ) {
	int a, b, c, d;
	Uint8 * dotQuad = (Uint8*)&ip;
	a = (unsigned short)dotQuad[0];
	b = (unsigned short)dotQuad[1];
	c = (unsigned short)dotQuad[2];
	d = (unsigned short)dotQuad[3];
	char *result = (char*) malloc(16);
	sprintf(result, "%i.%i.%i.%i", a, b, c, d);
	return result;
}

const char * getStringAddress( IPaddress ip ) {
	char * result = (char*) malloc(24);
	sprintf(result, "%s:%i", getStringIP(ip.host), ip.port);
	return result;
}