#include "Server_References.h"

extern void broadcast( char type, const void* data, char len );

void update(float dt) {
	int clientsCount = clients.size();
	lockMutex( clientsMutex );
	for ( Uint32 i = 0; i < clients.size(); i++ ) {
		lockMutex( clients[i]->mutex );
		if ( clients[i]->recvSize > 2 ) {
			int readPos = clients[i]->startPos;
			if ( readPos >= BUFFER_SIZE )
				readPos -= BUFFER_SIZE;
			char msgType = clients[i]->msg[readPos];
			readPos++;
			if ( readPos >= BUFFER_SIZE )
				readPos -= BUFFER_SIZE;
			char msgLength = clients[i]->msg[readPos];
			if ( msgLength >= clients[i]->recvSize - 2 ) {
				char * data = (char*) malloc( msgLength+1 );
				for ( Uint16 j = 0; j < msgLength; j++ ) {
					readPos++;
					if (readPos >= BUFFER_SIZE)
						readPos -= BUFFER_SIZE;
					data[j] = clients[i]->msg[readPos];
				}
				readPos++;
				if ( readPos >= BUFFER_SIZE )
					readPos -= BUFFER_SIZE;
				clients[i]->startPos  = readPos;
				clients[i]->recvSize -= msgLength+2;

				data[msgLength] = 0;
				switch ( msgType ) {
					case 'm':
						printf( "%s: %s\n", clients[i]->address, data );
					break;
					case 'b':
						broadcast( 'm', data, strlen(data) );
					break;
				}
			}
		}
		unlockMutex( clients[i]->mutex );
	}
	unlockMutex( clientsMutex );
}
