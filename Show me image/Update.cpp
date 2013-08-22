#include "refers.h"


void update( float dt ){
	if ( fly )
		 fly->Update();
	if ( anotherFly )
		 anotherFly->Update();
	anim->Update( dt );
}