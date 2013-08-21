#ifndef _FLY_
#define _FLY_
#include "Animaiton.h"

class Fly {
public:
	Fly( float x_, float y_ );
	void	Move	();
	void	Render	();
	void	Turn	( signed char dir );
	void	TurnZ	( signed char dir );
	void	GetXY	( float &_x, float &_y );
	float	GetZ	();
	void	SetAnimation ( BAnimation * anim );
	void	Stop	();
	float	camera_max_zoom;
	float	camera_min_zoom;
protected:
	BAnimation *animation;
	float	x, y, z;
	float	xpre, ypre, zpre;
	float	speed;
	float	angle_rad;
	float	angular_speed;
	float	turn_dir;
	char	z_dir;
};

#endif