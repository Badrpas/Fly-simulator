#include "Server_References.h"

Client* getClientByID ( Uint8 id_){
	for (unsigned int i = 0; i < clients.size(); i++)
		if ( id_ == clients.at(i)->ID ) 
			return clients.at(i);
	return NULL;
}

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
			if ( msgLength <= clients[i]->recvSize - 2 ) {
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
				void* dat = data;
				printf( "Decoded message type %c:%X\n", msgType, (unsigned)msgType );
				switch ( msgType ) {
					case 'm':
						printf( "%s: %s\n", clients[i]->address, data );
						break;
					case 'b':
						broadcast( 'm', data, msgLength );
						break;
					case B_NEWPLAYER:{
						unsigned int id = clients[i]->ID;
						send ( &clients[i]->socket, B_NEWPLAYERID, &id, sizeof( unsigned int ) );
						float* pos = (float*) data;
						pos[3] = (float) clients[i]->ID;
						printf("New player. ID: %i\n", clients[i]->ID);
						broadcastExcept( clients[i]->socket, B_NEWPLAYER, data, sizeof(float)*4 );
						break;
						}
					case B_FORNEWBIE:{
						Uint8 id = ((float*)dat)[8];
						Client* dest = getClientByID(id);
						if (dest != NULL) {
							send( &dest->socket, B_FORNEWBIE, dat, msgLength );
						}
						break;
					}
					case B_PLAYERTURN:{
						broadcastExcept ( clients[i]->socket, B_PLAYERTURN, dat, msgLength );
						break;
					}
					case B_PLAYERTURNZ:
						broadcastExcept ( clients[i]->socket, B_PLAYERTURNZ, dat, msgLength );
						break;
				}
			}
		}
		unlockMutex( clients[i]->mutex );
	}
	unlockMutex( clientsMutex );
}
