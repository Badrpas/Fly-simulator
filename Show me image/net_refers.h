#include <SDL_net.h>


extern bool terminated;

extern void send(TCPsocket * sock, char type, const void* data, char len);

extern int unlockMutex	( SDL_mutex * mutex );
extern int lockMutex	( SDL_mutex * mutex );

extern int decodeData	( void* );
extern int recvData		( void* );
