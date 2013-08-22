#include "refers.h"

BAnimation::BAnimation ( float width_, float height_, float delay_ ) {
	width			= width_;
	height			= height_;
	delay			= delay_;
	time			= 0;
	frameNumber		= 0;
}

void BAnimation::AddFrame ( GLuint texture ) {
	frames.push_back ( texture );
}

void BAnimation::Update ( float dt ) {
	time += dt;
	frameNumber = getFrameNumber() % frames.size() ;
}

void BAnimation::Render (	float x,	float y, 
							float w,	float h,
							BRadians r, 
							float dx,	float dy) {
	if ( frames.size() > 0 ) {
		float _w, _h;
		if ( w == 0 ) {
			_w = width;
		} else {
			_w = w;
		}
		if ( h == 0 ) {
			_h = height;
		} else {
			_h = h;
		}
		_h = h;
		_w = w;
		draw ( frames.at( frameNumber ), x, y, _w, _h, r, dx, dy );
	}
}

void BAnimation::Render ( float x, float y ) {
	if ( frames.size() > 0 ) {
		draw ( frames.at( frameNumber ), x, y, width, height );
	}
}

unsigned int BAnimation::getFrameNumber () {
	float f;
	float r;
	f = modf ( time / delay, &r );
	return ( unsigned int ) r;
}