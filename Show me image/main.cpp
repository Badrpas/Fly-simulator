#include "refers.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 600;
const int SCREEN_BPP = 32;
const float VIEW_RANGE_MOD = 0.98f;



SDL_Surface *image	= NULL;
SDL_Surface *screen	= NULL;
SDL_Surface *flyImg	= NULL;
SDL_Surface *kover	= NULL;

GLuint glFlyTexture;
GLuint glKoverTexture;

BAnimation *anim;

Uint32 FLAGS;

SDL_Event event;
bool minimapEnabled = true;
Fly *fly = NULL;


int mouseX, mouseY;
int mouseXrel, mouseYrel;
int mouseXpre = 0, mouseYpre = 0;

float dt;

int main( int argc, char* args[] ) {
    bool quit = false;
	init();
	load_files();
	float preTime;
	float curTime;
	preTime = SDL_GetTicks();
	
	fly = new Fly( 550.0f, 324.0f );
	anim = new BAnimation ( 64, 64, 0.03f );
	anim->AddFrame ( glFlyTexture );
	anim->AddFrame ( loadTexture ( "images/Fly/fly_fly1.png", GL_RGBA, GL_NEAREST ) );
	anim->AddFrame ( loadTexture ( "images/Fly/fly_fly2.png" ) );
	anim->AddFrame ( loadTexture ( "images/Fly/fly_fly1.png" ) );
	fly->SetAnimation ( anim );


	while ( quit == false ) {
		while ( SDL_PollEvent( &event ) ) {
			if ( event.type == SDL_QUIT ) {
                quit = true;
            } else if ( event.type == SDL_MOUSEMOTION ) {
				if ( length( mouseXpre, mouseYpre, mouseX, mouseY ) >= 20.0f ) {
					mouseXpre = mouseX;
					mouseYpre = mouseY;
				}
				mouseX = event.motion.x;
				mouseY = event.motion.y;
				mouseXrel = event.motion.xrel;
				mouseYrel = event.motion.yrel;
			} else if ( event.type == SDL_MOUSEBUTTONDOWN ) {
			} else if ( event.type == SDL_KEYDOWN ) {
				SDLKey key = event.key.keysym.sym;
				if ( key == SDLK_ESCAPE )
						quit = true;
				if ( key == SDLK_f ) {
					FLAGS ^= SDL_FULLSCREEN;
					screen = SDL_SetVideoMode( 
						SCREEN_WIDTH, SCREEN_HEIGHT, 
						SCREEN_BPP, FLAGS );
				}
				if ( key == SDLK_RIGHT )
					fly->Turn ( 1 );
				if ( key == SDLK_LEFT  )
					fly->Turn (-1 );
				if ( key == SDLK_UP )
					fly->TurnZ ( -1 );
				if ( key == SDLK_DOWN )
					fly->TurnZ ( +1 );
				if ( key == SDLK_m )
					minimapEnabled = ! minimapEnabled;


			} else if ( event.type == SDL_KEYUP ) {
				SDLKey key = event.key.keysym.sym;		
				if ( key == SDLK_RIGHT )
					fly->Turn ( -1 );
				if ( key == SDLK_LEFT  )
					fly->Turn ( +1 );
				if ( key == SDLK_UP )
					fly->TurnZ ( +1 );
				if ( key == SDLK_DOWN )
					fly->TurnZ ( -1 );
			}				
		}

		curTime = SDL_GetTicks();
		dt = ( curTime - preTime ) / 1000;
		preTime = curTime;

		update(dt);
		render();

	}



	clean_up();
	return 0;    
}