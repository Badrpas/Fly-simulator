#include "refers.h"


void update( float dt ){
	if (fly)
		fly->Move();
	anim->Update( dt );
}