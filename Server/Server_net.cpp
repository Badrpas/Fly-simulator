#include "Server_References.h"


void cleanClients();


int clientNetLoop(void *) {
	int count = 0;
	while( !terminated ) { 
		int numActiveSockets		= SDLNet_CheckSockets( socketSet, 1 );
		int serverSocketActivity	= SDLNet_SocketReady ( socket );

		if (serverSocketActivity != 0) {
			TCPsocket newSocket = SDLNet_TCP_Accept( socket );
			SDLNet_TCP_AddSocket( socketSet, newSocket );
			Client * newClient = new Client( &newSocket );
			lockMutex  ( clientsMutex );
			clients.push_back( newClient );
			unlockMutex( clientsMutex );
			count++;
			printf( "%s connected\n", newClient->address );
			char*	helloMsg = "Hello, my dear!";
			send( &newSocket, 'm', helloMsg, strlen( helloMsg ) );
			char*	nameMsg	 = (char*) GenerateName();
			send( &newSocket, 'n', nameMsg, strlen(nameMsg) );
		}

		for (unsigned int i = 0; i < clients.size(); i++) {
			int clientSocketActivity = SDLNet_SocketReady( clients[i]->socket );
			if (clientSocketActivity != 0 ) {
				char * buffer = (char*) malloc( BUFFER_SIZE - clients[i]->recvSize );
				Uint32 recvByteCount = SDLNet_TCP_Recv( 
											clients[i]->socket, 
											buffer, 
											BUFFER_SIZE - clients[i]->recvSize );
                if ( recvByteCount <= 0 ) {
					printf( "%s disconnected\n", clients[i]->address );
					SDLNet_TCP_DelSocket( socketSet, clients[i]->socket );
					SDLNet_TCP_Close( clients[i]->socket );
                    clients[i]->socket = NULL;
                    count--;
                } else {
					lockMutex( clients[i]->mutex );
					int writePos = clients[i]->startPos + clients[i]->recvSize;
					for ( unsigned int j = 0; j < recvByteCount; j++ ) {
						if (writePos >= BUFFER_SIZE)
							writePos -= BUFFER_SIZE;
						clients[i]->msg[writePos] = buffer[j];
						writePos++;
					}
					clients[i]->recvSize += recvByteCount;
					unlockMutex( clients[i]->mutex );
                }
            }
		}
		cleanClients();
	}
	return count;
}

void cleanClients() {
	int disconnectedCount = 0;
	lockMutex( clientsMutex );
	for ( Uint32 j = 0; j < clients.size(); j++ ) {
		if ( clients[j]->socket == NULL ){
			disconnectedCount++;
			std::swap( clients[j], clients[clients.size() - disconnectedCount] );
		}
	}
	if ( disconnectedCount ) {
		clients.resize( clients.size() - disconnectedCount );
	}
	unlockMutex( clientsMutex );
}
