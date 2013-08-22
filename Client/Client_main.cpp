#define _CRT_SECURE_NO_WARNINGS

#include "Client_references.h"

bool	DEBUG_MODE = false;
bool	terminated = false;

IPaddress ip;
TCPsocket socket;
SDLNet_SocketSet socketSet = SDLNet_AllocSocketSet(1);

SDL_mutex *mutex = SDL_CreateMutex();

char*	msg;
int		recvSize = 0;
int		startPos = 0;
int		endPos	 = 0;
const short BUFFER_SIZE = 1024;



void send(TCPsocket * sock, char type, const void* data, char len){
	SDLNet_TCP_Send(*sock, &type,1  );
	SDLNet_TCP_Send(*sock, &len, 1  );
	SDLNet_TCP_Send(*sock, data, len);
}

int recvData(void*);

int decodeData(void*);

int main( int argc, char * argv[] ) {
	printf("Initializing SDL system... ");
			if(SDL_Init(SDL_INIT_EVERYTHING)==-1)	{printf("\nSDL_Init: %s\n",		SDL_GetError());	exit(1);}
			if(SDLNet_Init()==-1)					{printf("\nSDLNet_Init: %s\n",	SDLNet_GetError());	exit(2);}	
	printf("ok.\n");
	
	int port = 0;

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
		if ( argc == 3 ) {
			sscanf( argv[2], "%i", &port );
		}
	} else
		ip.host = convFromIP(127,0,0,1);
	ip.port = port;
	
	printf ( "done\n" );

	//SDLNet_ResolveHost( &ip, "badrpas.com", 3879 );

	const char * host = SDLNet_ResolveIP( &ip );

	printf( "Resolved host: %s\n", host);

	
	printf("Connecting to %s... ", getStringAddress(ip));
	socket = SDLNet_TCP_Open(&ip);
	if(!socket) {
		printf("\nSDLNet_TCP_Open: %s\n", SDLNet_GetError());
		exit(2);
	}
	printf("ok.\n");

	SDLNet_TCP_AddSocket(socketSet, socket);

	msg = (char*) malloc(BUFFER_SIZE+1);
	msg[BUFFER_SIZE+1] = 0;
	printf("\n");

	SDL_Thread *recvThread;
	SDL_Thread *decodeThread;

	recvThread		= SDL_CreateThread( recvData,	NULL );
	decodeThread	= SDL_CreateThread( decodeData,	NULL );

	char in = 0;
	for( ;; ) { 
		if ( terminated )
			break;
		scanf( "%c", &in );
		if ( in == 'd' ) {
			DEBUG_MODE = !DEBUG_MODE;
			printf( "DEBUG_MODE is now %s\n", 
					(DEBUG_MODE ? "ENABLED" : "DISABLED" ) );
		}
		if ( in == 'q' ) {
			terminated = true;
		}
		if ( in == 's' ) {
			char * sendMsg = (char*) malloc(255);
			scanf( "%[' 'A-z'\''!-+]s", sendMsg );
			send (&socket, 'm', sendMsg, strlen(sendMsg));
		}		
		if ( in == 'b' ) {
			char * sendMsg = (char*) malloc(255);
			scanf( "%[' 'A-z'\''!-+]s", sendMsg );
			send( &socket, 'b', sendMsg, strlen(sendMsg) );
		}
		if ( in == 'w' ) {
			printf( "%i:%i(%c):",startPos, recvSize, msg[startPos] );
			for ( int i = 0; i < BUFFER_SIZE; i++ )
				printf( "%c", msg[i] );
			printf( "\n" );
		}
	}
	
	printf("\n");
	printf("Terminated\n");

	int *result;
	result = (int*)malloc(1);
	printf("Waiting for recieve thread end\n");
	SDL_WaitThread(	  recvThread, result);
	printf("Waiting for decode thread end\n");
	SDL_WaitThread(	decodeThread, result);
	printf("Close socket\n");
	SDLNet_TCP_Close(socket);

	return 0xFEEDED;
}

int recvData(void*) {
	for (;!terminated;) {

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

int decodeData(void*){
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