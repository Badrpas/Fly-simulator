#include "refers.h"

void calculate(){

}



SDL_Surface *load_image( char * filename ) {
    SDL_Surface* loadedImage	= NULL;
    SDL_Surface* optimizedImage	= NULL;
	loadedImage = IMG_Load( filename );
	return loadedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination ) {
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;

    SDL_BlitSurface( source, NULL, destination, &offset );
}

bool initGL() {
	glClearColor((128.0f / 255.0f), 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthMask( GL_TRUE );
	glDepthFunc( GL_LEQUAL );
	glDepthRange( 1.0f, -1.0f );
	glClearDepth( 1.0f );
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho( 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 1.0, -1.0 );
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);										// Here is magic
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );	// for alpha-blending
	FILE * file;
	file = fopen( "opengl.log", "w" );
    GLenum error = glGetError();
    if( error != GL_NO_ERROR ) {
        fprintf( file, "Error initializing OpenGL! %s\n", gluErrorString( error ) );
        printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
        return false;
    } else {
        fprintf( file, "All ok.\n" );
	}
	fclose( file );
    return true;
}

bool init() {
	printf ( "Hello\n" );
	FLAGS = SDL_HWSURFACE | SDL_GL_DOUBLEBUFFER | SDL_OPENGL;
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 ) {
        return false;    
    }
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,        8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,      8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,       8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,      8);
 
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,      16);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,		32);
 
	SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE,    8);
	SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE,  8);
	SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE,   8);
	SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE,  8);
 
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,  1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,  2);

    screen = SDL_SetVideoMode( 
		SCREEN_WIDTH, SCREEN_HEIGHT, 
		SCREEN_BPP, FLAGS );
	initGL();
	
	printf( "Vendor: %s\n", glGetString( GL_VENDOR ) );
	printf( "Renderer: %s\n", glGetString( GL_RENDERER ) );
	printf( "GL version: %s\n", glGetString( GL_VERSION ) );
	printf( "SL version: %s\n", glGetString( GL_SHADING_LANGUAGE_VERSION ) );

    if( screen == NULL ) {
        return false;
    }
	SDL_WM_SetCaption( "Event test", NULL );
    return true;
}

GLuint convToGLTex (SDL_Surface * surface, 
					GLuint &result,
					GLint glFormat = GL_RGBA, 
					GLint filter   = GL_LINEAR, 
					GLint wrapping = GL_CLAMP) {
	glGenTextures ( 1, &result );
	glBindTexture ( GL_TEXTURE_2D, result );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping );
	glTexImage2D (
			GL_TEXTURE_2D, 
			0, glFormat, 
			surface->w, surface->h, 
			0, glFormat, 
			GL_UNSIGNED_BYTE, 
			surface->pixels );
	return result;
}

GLuint loadTexture (char * file,
					GLint glFormat , 
					GLint filter   , 
					GLint wrapping ) {
	GLuint texture = 0;
	SDL_Surface *surface;
	surface = load_image ( file );
	glGenTextures ( 1, &texture );
	glBindTexture ( GL_TEXTURE_2D, texture );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping );
	glTexImage2D (
			GL_TEXTURE_2D, 
			0, glFormat, 
			surface->w, surface->h, 
			0, glFormat, 
			GL_UNSIGNED_BYTE, 
			surface->pixels );

	return texture;
}

bool load_files() {
    image	= load_image( "images/hello.png" );
	flyImg	= load_image( "images/fly.png" );
	kover	= load_image( "images/KOVER.png" );
	convToGLTex ( kover, glKoverTexture, GL_RGBA, GL_LINEAR, GL_MIRRORED_REPEAT );
	convToGLTex ( flyImg, glFlyTexture , GL_RGBA );
	return true;
}

void clean_up() {
    SDL_FreeSurface( image );
    SDL_Quit();    
}

float getAngle( float X1, float Y1, float X2, float Y2 ) {
	float X;
	float Y;
	X=X2-X1;
	Y=Y2-Y1;
	if(X==0){
		if ( Y >= 0 ) return M_PI;
		else return 0;
	}
	else{
		if(X>0)
			return atan(Y/X) + M_PI_2;	
		else
			if(Y>=0)
				return atan(Y/X) + M_PI + M_PI_2; 
			else
				return atan(Y/X) - M_PI_2; 
	}
}

float length( float X1, float Y1, float X2, float Y2 ) {
	return sqrt( (X1-X2)*(X1-X2)+(Y1-Y2)*(Y1-Y2) );
}

void  getCoordsMove(	float &x,		float &y, 
						float angle,	float length	) {
	x -= length*sin(angle-M_PI) * 1.5;
	y += length*cos(angle+M_PI);
}

void  getCoordsMove(	float &x,		float &y, 
						float angle,	float length, 
						SDL_Rect bounds					) {
	float _x = x, _y = y;
	x -= length*sin(angle-M_PI) * 1; // SCREEN_WIDTH/SCREEN_HEIGHT
	if ( x < bounds.x || x > bounds.x + bounds.w ) {
		x = _x;
	}
	y += length*cos(angle+M_PI);
	if ( y < bounds.y || y > bounds.y + bounds.h ) {
		y = _y;
	}
}
