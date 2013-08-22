#include <SDL.h>
#include <SDL_image.h>
#include <SDL_opengl.h>
#include <SDL_net.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

#include "Fly.h"
#include "Animaiton.h"
#include "net_refers.h"

extern SDL_Rect bViewPort;

extern Fly* fly;
extern Fly* anotherFly;

extern BAnimation * anim;


extern float dt;


extern int mouseX   , mouseY;
extern int mouseXpre, mouseYpre;
extern int mouseXrel, mouseYrel;

extern float viewRange;

extern Uint32 FLAGS;

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int SCREEN_BPP;

extern bool minimapEnabled;

extern SDL_Surface *screen;
extern SDL_Surface *image;
extern SDL_Surface *flyImg;
extern SDL_Surface *kover;

extern GLuint glFlyTexture;
extern GLuint glKoverTexture;

extern SDL_Surface *load_image ( char * filename );

extern GLuint convToGLTex ( SDL_Surface * surface, 
							GLint glFormat, 
							GLint filter );
extern GLuint loadTexture (	char * file,
							GLint glFormat = GL_RGBA, 
							GLint filter   = GL_LINEAR, 
							GLint wrapping = GL_CLAMP 	);

extern double B_ALPHA_VALUE;

extern void draw(GLuint texture, 
		float x, float y, 
		float w, float h, 
		float r = 0, 
		float dx = 0, 
		float dy = 0);
extern void drawKover();

extern void apply_surface( int x, int y, 
		SDL_Surface* source, SDL_Surface* destination );
extern bool init();
extern void clean_up();
extern bool load_files();
extern void calculate();

extern float getAngle (	float X1, float Y1, float X2, float Y2 );
extern float length   (	float X1, float Y1, float X2, float Y2 );
extern void  getCoordsMove( float &x,	float &y, 
							float angle,float length );
extern void  getCoordsMove(	float &x,	float &y, 
							float angle,float length, 
							SDL_Rect bounds	);
extern void render();
extern void update( float dt );
