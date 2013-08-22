#define _CRT_SECURE_NO_WARNINGS
#include "refers.h"

bool terminated;

int port;


SDL_Thread *recvThread;
SDL_Thread *decodeThread;

IPaddress ip;
TCPsocket socket;
SDLNet_SocketSet socketSet = SDLNet_AllocSocketSet(1);

SDL_mutex *mutex = SDL_CreateMutex();

char*	msg;
int		recvSize = 0;
int		startPos = 0;
int		endPos	 = 0;
const short BUFFER_SIZE = 1024;


void initNet ( int argc, char* argv[] ) {
	if ( SDLNet_Init() == -1 ) {
		printf( "\nSDLNet_Init: %s\n",	SDLNet_GetError() );	
		exit(2);
	} else
		printf( "SDL_Net initialized.\n" );
	printf ( "Reading arguments " );
	if (argc > 1) {
		char count = 0;
		for ( unsigned int i = 0; i < strlen(argv[1]); i++ )
			if (argv[1][i] == '.') count++;
		if (count = 3) {
			int a, b, c, d;
			a = b = c = d = 0;
			sscanf( argv[1], "%i%*c%i%*c%i%*c%i", &a, &b, &c, &d );
			ip.host = convFromIP(a,b,c,d);
			if ( argc == 3 ) {
				sscanf( argv[2], "%i", &port );
			}
		} else {
			printf( "Incorrect ip argument. Using localhost address.\n" );
			ip.host = convFromIP(127,0,0,1);
		}
	} else
		ip.host = convFromIP(127,0,0,1);
	ip.port = port;
	printf ( "done\n" );
	const char * host = SDLNet_ResolveIP( &ip );
	printf( "Resolved host: %s\n", host);
	printf( "Connecting to %s...", getStringAddress(ip));
	socket = SDLNet_TCP_Open(&ip);
	if ( !socket ) {
		printf("\nSDLNet_TCP_Open: %s\n", SDLNet_GetError());
		exit(2);
	} else {
		NETWORK = true;
		printf("ok.\n");
		SDLNet_TCP_AddSocket(socketSet, socket);
		msg = (char*) malloc(BUFFER_SIZE+1);
		msg[BUFFER_SIZE+1] = 0;
		recvThread		= SDL_CreateThread( recvData,	NULL );
		decodeThread	= SDL_CreateThread( decodeData,	NULL );
	}

}


Uint32 convFromIP( int a, int b, int c, int d ) {
	return a + (b << 8) + (c << 16) + (d << 24);
}
Uint32 convFromIP( char * address ) {
	Uint8 a, b, c, d;
	sscanf( address, "%i%*c%i%*c%i%*c%i", &a, &b, &c, &d );
	return a + (b << 8) + (c << 16) + (d << 24);
}

const char * getStringIP ( Uint32 ip ) {
	int a, b, c, d;
	Uint8 * dotQuad = (Uint8*) &ip;
	a = (unsigned short) dotQuad[0];
	b = (unsigned short) dotQuad[1];
	c = (unsigned short) dotQuad[2];
	d = (unsigned short) dotQuad[3];
	char *result = (char*) malloc(16);
	sprintf( result, "%i.%i.%i.%i", a, b, c, d );
	return result;
}

const char * getStringAddress( IPaddress ip ) {
	char * result = (char*) malloc(24);
	sprintf(result, "%s:%i", getStringIP(ip.host), ip.port);
	return result;
}

void send ( char type, const void* data, char len ) {
	SDLNet_TCP_Send(socket, &type,1  );
	SDLNet_TCP_Send(socket, &len, 1  );
	SDLNet_TCP_Send(socket, data, len);
}



int recvData( void* ) {
	for ( ;!terminated; ) {

		int active  = SDLNet_CheckSockets( socketSet, 10);
		if( active != 0 ){

			lockMutex(mutex);
			int writePos = startPos + recvSize;
			if ( writePos >= BUFFER_SIZE )
				writePos -= BUFFER_SIZE;
			char* buf = (char*) malloc (256);
			int r = SDLNet_TCP_Recv(socket, buf, 1);
			if ( r <= 0 ) {
				printf( "Server closed.\n" );
				terminated = true;
				exit(0);
				return 0;
			} else {
				for ( int i = 0; i < r; i++ ) {
					msg[writePos] = buf[i];
					writePos++;
					if ( writePos >= BUFFER_SIZE )
						writePos -= BUFFER_SIZE;
				}
			}
			recvSize++;
			unlockMutex(mutex);
		}
	}
	return 0;
}

Fly* getFlyByID ( Uint8 id_){
	for (unsigned int i = 0; i < otherFlies.size(); i++)
		if ( id_ == otherFlies.at(i)->GetID() ) 
			return otherFlies.at(i);
	return NULL;
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
					void* data = messageData;
					if ( startPos >= BUFFER_SIZE )
						startPos -= BUFFER_SIZE;
					switch ( messageType ) {
						case 'm':
							printf( "Server: %s\n", messageData );
							break;
						case 'n':
							printf( "Server set your name to \"%s\"\n", messageData );
							break;
						case B_NEWPLAYER:{
							float* pos = (float*) data;
							printf( "New player (%.3f; %.3f) ID:%i \n", pos[0], pos[1], (int)pos[3] );
							otherFlies.push_back( new Fly( pos[0], pos[1] ) );
							otherFlies.at( otherFlies.size()-1 )->SetID( (unsigned int) pos[3] );
							fly->SendStateToID ( (unsigned int) pos[3] );
							break;
							}
						case B_NEWPLAYERID:
							fly->SetID( (unsigned) messageData[0] );
							printf( "My ID: %i\n", fly->GetID() );
							break;
						case B_FORNEWBIE:{
							float* pos = (float*) data;
							printf( "Existing player at (%.3f; %.3f) ID:%i \n", pos[0], pos[1], (Uint8)pos[7] );
							otherFlies.push_back( new Fly( pos ) );
							break;
							}
						case B_PLAYERTURN:{
							float * dat = (float*) data;
							printf( "Other Fly turned at %.3f %.3f\n", dat[0], dat[1]);
							Fly* who = getFlyByID( (Uint8) ( dat[4] ) );
							if ( who != NULL ) {
								who->SetPosition(dat[0], dat[1], dat[2], dat[3]);
							}
							break;
						}
						case B_PLAYERTURNZ:{
							float * dat = (float*) data;
							printf( "Other Fly turned Z(%i) at %.3f %.3f\n", dat[3], dat[0], dat[1] );
							Fly* who = getFlyByID( (Uint8) ( dat[4] ) );
							if ( who != NULL ) {
								who->SetPositionZ(dat[0], dat[1], dat[2], dat[3]);
							}
							break;
						}
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