#ifndef _ANIMATION_
#define _ANIMATION_

typedef unsigned int Uint;

typedef float BRadians;
typedef float BDegrees;

class BAnimation{
public:
			BAnimation		( float width_, float height_, float delay_ );
	void	AddFrame		( GLuint texture );
	void	Update			(	float dt );
	void	Render			(	float x, float y, float w, float h,
								BRadians r = 0, float dx= 0, float dy = 0 );
	void	Render			(	float x, float y );
	Uint	getFrameNumber	();
protected:
	std::vector < GLuint > frames;
	float	width, height;
	float	time, delay;
	Uint	frameNumber;
};

#endif