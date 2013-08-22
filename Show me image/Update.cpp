#include "refers.h"


void update( float dt ){
	if ( fly )
		 fly->Update();
	if ( anotherFly )
		 anotherFly->Update();
	for ( int i = 0; i < otherFlies.size(); i++ ) {
			otherFlies.at( i ) -> Update();
	}
	anim->Update( dt );
}