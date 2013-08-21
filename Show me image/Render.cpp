#include "refers.h"

void render() {
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	drawKover();
	//anim->Render ( 100, 100 );
	if (fly)
		fly->Render();
    SDL_GL_SwapBuffers();
}