#include <SDL_net.h>
#include "Send_const.h"


extern void				initNet		( int argc, char* argv[] );
extern bool				terminated; 
extern IPaddress		ip;
extern TCPsocket		socket;
extern SDLNet_SocketSet	socketSet;
extern SDL_mutex*		mutex;
extern char*			msg;
extern int				recvSize;
extern int				startPos;
extern int				endPos;
extern short const		BUFFER_SIZE;
extern Uint32			convFromIP	( int a, int b, int c, int d );
extern Uint32			convFromIP	( char * address );
extern const char*		getStringIP	( Uint32 ip );
extern const char*		getStringAddress	( IPaddress ip );
extern void				send		( char type, const void* data, char len);
extern int				unlockMutex	( SDL_mutex * mutex );
extern int				lockMutex	( SDL_mutex * mutex );
extern int				decodeData	( void* );
extern int				recvData	( void* );

extern SDL_Thread *recvThread;
extern SDL_Thread *decodeThread;
