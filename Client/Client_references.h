#include <SDL_net.h>
#define _CRT_SECURE_NO_WARNINGS


extern			Uint32	convFromIP(int a, int b, int c, int d);
extern const	char*	getStringIP( Uint32 ip );
extern const	char*	getStringAddress( IPaddress ip );
extern			int		lockMutex  ( SDL_mutex * mutex );
extern			int		unlockMutex( SDL_mutex * mutex );

