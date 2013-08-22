#ifndef _FLY_
#define _FLY_
#include "Animaiton.h"


class Fly {
public:
					Fly					( float x_, float y_ );
					Fly					( float* data );
		void		Update				();
		void		Render				();
		void		Turn				( signed char dir );
		void		TurnZ				( signed char dir );
		void		GetXY				( float &_x, float &_y );
		float		GetZ				();
		void		Stop				();
		bool		IsPlayer			();
		bool		IsMainPlayer		();
		void		SetSpeed			( float speed_);
		void		SetPlayerState		( bool state );
		void		SetMainPlayerState	( bool state );
		void		SetAnimation		( BAnimation * anim );
		void		SetPosition			( float x_, float y_, BRadians angle_, char turn_dir_ );
		void		SetPositionZ		( float x_, float y_, float z_, char z_dir_ );
		void		SetZ				( float z_ );
		void		SetID				( unsigned int id_ );
		Uint		GetID				();
		void		SendNewPlayer		();
		void		SendStateToID		( unsigned int id_ );

protected:
		void		Move				();
		float		x, y, z;
		float		xpre, ypre, zpre;
		float		speed;
		float		camera_max_zoom;
		float		camera_min_zoom;
		bool		isMainPlayer;
		bool		isPlayer;
		char		turn_dir;
		char		z_dir;
		char		id;
		BRadians	angle_rad;
		BRadians	angular_speed;
		BAnimation* animation;
};

#endif