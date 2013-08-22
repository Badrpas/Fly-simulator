#ifndef _FLY_
#define _FLY_
#include "Animaiton.h"

typedef float BRadians;
typedef float BDegrees;

class Fly {
public:
					Fly			( float x_, float y_ );
		void		Update		();
		void		Render		();
		void		Turn		( signed char dir );
		void		TurnZ		( signed char dir );
		void		GetXY		( float &_x, float &_y );
		float		GetZ		();
		void		Stop		();
		bool		IsPlayer	();
		bool		IsMainPlayer();
		void		SetSpeed			( float speed_);
		void		SetPlayerState		( bool state );
		void		SetMainPlayerState	( bool state );
		void		SetAnimation		( BAnimation * anim );
		void		SetPosition			( float x_, float y_, BRadians angle_ );

protected:
		void		Move	();
		float		x, y, z;
		float		xpre, ypre, zpre;
		float		speed;
		float		camera_max_zoom;
		float		camera_min_zoom;
		bool		isMainPlayer;
		bool		isPlayer;
		char		turn_dir;
		char		z_dir;
		BAnimation* animation;
		BRadians	angle_rad;
		BRadians	angular_speed;
};

#endif