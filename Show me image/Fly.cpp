#include "refers.h"

Fly::Fly( float x_, float y_ ) {
	x = x_;
	y = y_;
	z = 1.0f;
	speed = 150.0f;
	angle_rad = 30.0f;
	angular_speed = M_PI_2 * 1.5f;
	turn_dir = 0;
	z_dir = 0;
	animation = NULL;
	camera_max_zoom = 0.07f;
	camera_min_zoom = 0.80f;
	isPlayer		= false;
	isMainPlayer	= false;
}

void	Fly::SetAnimation ( BAnimation * anim ) {
	animation = anim;
}

void	Fly::Render() {

	float _h, _w;
	float _x, _y;
	float _dx, _dy;

	const float H = 64.0f, const W = 64.0f;
	
	float length = GetZ() - fly->GetZ();


	if ( IsMainPlayer() ) {
		_h = W; _w = H;
		_x = x;	_y = y;
	} else {
		float amplitude = ( camera_min_zoom - camera_max_zoom ) / 6.0f ;
		float mod = 1.0f + length / amplitude;
		
		if ( mod < 0 )
			mod = 0;
		else if ( mod >= 1.5f )
			B_ALPHA_VALUE = 1.0f - 2.0f*(mod - 1.5f);
		else if ( mod <= 0.5f )
			B_ALPHA_VALUE = 2.0f*(mod);
		_h = H * mod;
		_w = W * mod;

		_x = x;	 _y = y;
		mod = 0;
		mod = 0;
		float xf, yf;
		fly->GetXY ( xf, yf );
		float zoom = 1 / fly->GetZ() ;
		
		_x = ( (x - bViewPort.x)/bViewPort.w ) * SCREEN_WIDTH;
		_y = ( (y - bViewPort.y)/bViewPort.h ) * SCREEN_HEIGHT;

	}
	
	_dx = _w / 2.0f;
	_dy = _h / 2.0f;
	if ( !animation )
		draw (	glFlyTexture, _x, _y, _w, _h, 
				angle_rad / M_PI * 180, _dx, _dy );
	else
		animation->Render ( _x, _y, _w, _h, angle_rad / M_PI * 180, _dx, _dy );
	B_ALPHA_VALUE = 1.0;
}

void	Fly::SetSpeed( float speed_ ){
	speed = speed_;
}

void	Fly::Turn ( signed char dir ) {
	if ( dir != 0 ) {
		if ( dir > 0 )
			turn_dir +=  1 ;
		else
			turn_dir -=  1 ;
	}
}
void	Fly::TurnZ ( signed char dir ) {
	if ( dir != 0 ) {
		if ( dir > 0 )
			z_dir +=  1 ;
		else
			z_dir -=  1 ;
	}
}

void	Fly::Update() {
	angle_rad += turn_dir * angular_speed * dt;	// turn de Fly
	Move();
	if ( z_dir )
		z += z_dir * dt * speed / 1000.0f;
	if ( z > camera_min_zoom )
		 z = camera_min_zoom;
	if ( z < camera_max_zoom )
		 z = camera_max_zoom;
}

void	Fly::Move  () {
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = SCREEN_WIDTH;
	rect.h = SCREEN_HEIGHT;
	getCoordsMove( x, y, angle_rad, speed*dt, rect);
}

float	Fly::GetZ  ()						{ return z; }
void	Fly::GetXY ( float &_x, float &_y ) { _x = x; _y = y; }


void	Fly::SetPosition ( float x_, float y_, BRadians angle_ ) {
	x = x_;		y = y_;
	angle_rad = angle_;
}


bool	Fly::IsMainPlayer	()	{ bool	r = isMainPlayer;	return	r;	}
bool	Fly::IsPlayer		()	{ bool	r = isPlayer;		return	r;	}	

void	Fly::SetPlayerState		( bool state )	{
	isPlayer = state;
}
void	Fly::SetMainPlayerState	( bool state )	{
	isMainPlayer = isPlayer = state;
}
