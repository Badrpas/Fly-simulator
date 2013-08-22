#include "Server_References.h"

extern const char * getStringAddress(IPaddress ip);

unsigned int IDCounter;

Client::Client(TCPsocket * socket_){
	socket = *socket_;
	msg = (char*) malloc(BUFFER_SIZE);
	ID = ++IDCounter;
	startPos = 0;
	recvSize = 0;
	mutex = SDL_CreateMutex();
	address = (char*) malloc(24);
	address = (char*) getStringAddress(*SDLNet_TCP_GetPeerAddress(socket));
}


void Client::recvData(){
	int writePos = startPos + recvSize;
	if (writePos >= BUFFER_SIZE)
		writePos -= BUFFER_SIZE;
	int r = SDLNet_TCP_Recv(socket, msg+writePos, 1);
	if (r <= 0){
		printf("Connection closed.\n");
	}
	recvSize++;
	printf("%c\n", msg[writePos]);
}
