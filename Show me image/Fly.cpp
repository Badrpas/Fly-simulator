#include "refers.h"

Fly::Fly( float x_, float y_ ) {
	x = x_;
	y = y_;
	z = 1.0f;
	speed = 100.0f;
	angle_rad = 30.0f;
	angular_speed = M_PI_2 * 1.5f;
	turn_dir = 0;
	z_dir = 0;
	animation = NULL;
	camera_max_zoom = 0.07f;
	camera_min_zoom = 0.80f;
}

void Fly::SetAnimation ( BAnimation * anim ) {
	animation = anim;
}

void Fly::Render() {
	if ( !animation )
		draw (	glFlyTexture, 
				x, y, 
				64.0f, 64.0f, 
				angle_rad / M_PI * 180, 
				32.0f, 32.0f );
	else
		animation->Render ( x, y, 0, 0, angle_rad / M_PI * 180, 32, 32 );
}

void Fly::Turn ( signed char dir ) {
	if ( dir != 0 ) {
		if ( dir > 0 )
			turn_dir +=  1 ;
		else
			turn_dir -=  1 ;
	}
}
void Fly::TurnZ ( signed char dir ) {
	if ( dir != 0 ) {
		if ( dir > 0 )
			z_dir +=  1 ;
		else
			z_dir -=  1 ;
	}
}
 
void Fly::Move  () {
	// turn fly
	angle_rad += turn_dir * angular_speed * dt;
	// and change it coords
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = SCREEN_WIDTH;
	rect.h = SCREEN_HEIGHT;
	getCoordsMove( x, y, angle_rad, speed*dt, rect);
	if ( z_dir )
			z += z_dir * dt * speed / 1000.0f;
	if ( z > camera_min_zoom )
		 z = camera_min_zoom;
	if ( z < camera_max_zoom )
		 z = camera_max_zoom;
	
}

void Fly::GetXY ( float &_x, float &_y ) {
	_x = x; _y = y;
}
float Fly::GetZ (){
	return z;
}