#pragma once


#include "../Name_Generator_lib/nameGen.h"
#include "../Show me image/Send_const.h"
#include <SDL_net.h>
#include <vector>

extern void broadcast( char type, const void* data, char len );
extern void broadcastExcept( TCPsocket exceptSock, char type, const void* data, char len );

extern Uint32 convFromIP( Uint8 a, Uint8 b, Uint8 c, Uint8 d );
extern const char * getStringIP( Uint32 ip );
extern const char * getStringAddress( IPaddress ip );

extern SDL_mutex*		clientsMutex;
extern int   lockMutex( SDL_mutex * mutex );
extern int unlockMutex( SDL_mutex * mutex );

extern void update(float dt);
extern void render(float dt);

extern IPaddress		ip;
extern TCPsocket		socket,			// Server
						clientSocket,	// Temporary
						lastSocket;		// Last connected
extern SDLNet_SocketSet socketSet;

extern void send(
	TCPsocket*	sock, 
	char		type, 
	const void* data, 
	char		len, 
	bool		outputAllowed = true);

extern	bool	terminated;

const	short	BUFFER_SIZE = 1024;

extern unsigned int IDCounter;

class Client {
public:
	Client( TCPsocket* socket_ );
	TCPsocket	socket;
	char*		msg;
	char*		address;
	int			startPos;
	int			recvSize;
	unsigned int ID;
	SDL_mutex*	mutex;

	void		recvData();
};

extern std::vector< Client* > clients;