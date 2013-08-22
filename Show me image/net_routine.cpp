#include "refers.h"

bool terminated;

IPaddress ip;
TCPsocket socket;
SDLNet_SocketSet socketSet = SDLNet_AllocSocketSet(1);

SDL_mutex *mutex = SDL_CreateMutex();

char*	msg;
int		recvSize = 0;
int		startPos = 0;
int		endPos	 = 0;
const short BUFFER_SIZE = 1024;



Uint32 convFromIP( int a, int b, int c, int d ) {
	return a + (b << 8) + (c << 16) + (d << 24);
}

const char * getStringIP ( Uint32 ip ) {
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

void send ( TCPsocket * sock, char type, const void* data, char len ) {
	SDLNet_TCP_Send(*sock, &type,1  );
	SDLNet_TCP_Send(*sock, &len, 1  );
	SDLNet_TCP_Send(*sock, data, len);
}


int recvData( void* ) {
	for ( ;!terminated; ) {

		int active  = SDLNet_CheckSockets( socketSet, 10);
		if( active != 0 ){

			lockMutex(mutex);
			int writePos = startPos + recvSize;
			if ( writePos >= BUFFER_SIZE )
				writePos -= BUFFER_SIZE;
			int r = SDLNet_TCP_Recv(socket, msg+writePos, 1);
			if ( r <= 0 ) {
				printf( "Server closed.\n" );
				terminated = true;
				exit(0);
				return 0;
			}
			recvSize++;
			unlockMutex(mutex);
		}
	}
	return 0;
}

int decodeData ( void* ) {
	char messageType	= 0;
	char messageLength	= 0;
	char *messageData;
	while ( !terminated ) {
			if (recvSize > 2) {

				lockMutex( mutex );
				messageType		= msg[startPos  ];
				messageLength	= msg[startPos+1 >= BUFFER_SIZE ? 1 :startPos+1];
				if (messageLength <= recvSize - 2) {
					messageData = (char*) malloc( messageLength+1 );
					int i = 0;
					for ( i = 0; i < messageLength; i++ ) {
						int readPos = startPos+2+i;
						if (readPos >= BUFFER_SIZE)
							readPos -= BUFFER_SIZE;
						messageData[i] = msg[readPos];
					}
					messageData[i] = 0;
					recvSize -= messageLength+2;
					startPos += messageLength+2;
					if ( startPos >= BUFFER_SIZE )
						startPos -= BUFFER_SIZE;
					switch ( messageType ) {
						case 'm':
							printf( "Server: %s\n", messageData );
						break;
						case 'n':
							printf( "Server set your name to \"%s\"\n", messageData );
						break;
					}
				}
				unlockMutex( mutex );

			} else
				SDL_Delay( 1 );
		
	}
	return 0;	
}

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