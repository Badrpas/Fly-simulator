#define _CRT_SECURE_NO_WARNINGS

#include "Server_References.h"


IPaddress ip;
TCPsocket socket; // Server socket
TCPsocket clientSocket = NULL, lastSocket = NULL;

SDL_mutex * clientsMutex = SDL_CreateMutex();
std::vector<Client*> clients;

SDLNet_SocketSet socketSet = SDLNet_AllocSocketSet(32);

SDL_Thread *acceptClientThread;
SDL_Thread *updateLoopThread;
SDL_Thread *renderLoopThread;

//const short BUFFER_SIZE = 1024;

bool terminated = false;

char msg[100] = "helloWorld!1234567890\0";

extern void update(float dt);


int clientNetLoop ( void * );

int workWithClient( void * socket_ );

int updateLoop( void * data_ ) {
	float dt = 0;
	for(;;)
		update(dt);
	return 0;
}

int renderLoop( void * data_ ) {
	return 0;
}
 
void send(	TCPsocket*	sock, 
			char		type, 
	const	void*		data, 
			char		len, 
			bool		outputAllowed ) {
	if ( *sock != NULL ) {
		IPaddress * clientIP = SDLNet_TCP_GetPeerAddress(*sock);
		if (outputAllowed) 
			printf("Sending -> %s : (%c:%X) \"%s\"\n", getStringAddress(*clientIP), type, type, data);
		SDLNet_TCP_Send(*sock, &type,1);
		SDLNet_TCP_Send(*sock, &len, 1);
		SDLNet_TCP_Send(*sock, data, len);
	}
}

void broadcast( char type, const void* data, char len ) {
	for (unsigned int i = 0; i < clients.size(); i++)
		send( &clients[i]->socket, type, data, len, true);
}
void broadcastExcept ( TCPsocket exceptSock, char type, const void* data, char len ) {
	for ( unsigned int i = 0; i < clients.size(); i++ )
		if ( exceptSock != clients[i]->socket )
			send ( &clients[i]->socket, type, data, len, true );
}

int main( int argc, char * argv[] ) {
	
	IDCounter = 0;
	unsigned char consoleWidth = 60;
	for (Uint8 k = 0; k < 10; k++) {
		const char * newName = GenerateName();
		if ( strlen (newName) > consoleWidth ) {
			consoleWidth = 80;
			printf("\n");
		}
		consoleWidth -= strlen (newName);
		printf( "%s ", newName );
	}
	printf( "\n" );

	printf( "Initializing SDL system... " );
			if(SDL_Init(SDL_INIT_EVERYTHING) == -1)	{printf("\nSDL_Init: %s\n", SDL_GetError());exit(1);	}
			if(SDLNet_Init() == -1 )				{printf("\nSDLNet_Init: %s\n", SDLNet_GetError());exit(2);}	
	printf( "ok.\n" );
	
	printf( "Starting server...\n" );
			//
			//
			if( SDLNet_ResolveHost(&ip, NULL, 3879) < 0 ) {
				printf( "\nSDLNet_ResolveHost: %s\n", SDLNet_GetError());
				exit(1);
			} else {
				//ip.host = convFromIP(127,0,0,1);
				ip.port = 3879;
				printf( "Resolved address: %s\n", getStringAddress(ip));
			}
			socket = SDLNet_TCP_Open( &ip );
			if(!socket ) {
				printf( "\nSDLNet_TCP_Open: %s\n", SDLNet_GetError());
				exit(2);
			}

	printf( "ok.\n");
	
	SDLNet_TCP_AddSocket( socketSet, socket );

	printf("Waiting for clients...\n");

	acceptClientThread = SDL_CreateThread(clientNetLoop, NULL);
	
	updateLoopThread = SDL_CreateThread(updateLoop, NULL);

	char instr = 0;
	for(;;) {
		scanf("%c", &instr);
		if (instr == 'q')
			break;
		if (instr == 'a') {
			send(&lastSocket, 'm', "yopt", 4);
		}
		if (instr == 'b') {
			char * broadcastMsg = (char*)malloc(255);
			broadcastMsg[0] = 1;
			broadcastMsg[1] = 0;
			scanf("%[' 'A-z'\'''!'-'+']s", broadcastMsg);
			broadcast('m', broadcastMsg, strlen(broadcastMsg));
		}
		if (instr == 'w') {
			printf("startPos: %i; recvSize: %i\n", clients[0]->startPos, clients[0]->recvSize);
			for (unsigned char i = 0; i < clients[i]->recvSize; i++) {
				if (	clients[0]->msg[i] != '\n' 
					&&	clients[0]->msg[i] != 10)
					printf("%3i\'%c\'; ", (unsigned char)clients[0]->msg[i], (unsigned char)clients[0]->msg[i]);
				else
					printf("%3i\' \'; ", (unsigned char)clients[0]->msg[i]);
			}
			printf("\n");
		}
		instr = 0;
	}
	terminated = true;
	printf("Terminated.\n");
	int status = 0;
	printf("Wait for accepting thread end. ");
	SDL_WaitThread(acceptClientThread,&status);
	printf("Ended.\nClosing socket. ");
	SDLNet_TCP_Close(socket);
	printf("Closed.\n");
	SDL_Quit();
	return 0x127;
}

