#include "refers.h"

void render() {
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glLoadIdentity();
	drawKover();
	//anim->Render ( 100, 100 );
	if ( anotherFly )
		 anotherFly->Render();
	for ( int i = 0; i < otherFlies.size(); i++ ) {
		otherFlies.at( i ) -> Render();
	}
	if (fly)
		fly->Render();
    SDL_GL_SwapBuffers();
}
