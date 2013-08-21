#include "refers.h"

void draw(	GLuint texture, 
			float x, float y, 
			float w, float h, 
			float r, 
			float dx,float dy ) {
	glBindTexture ( GL_TEXTURE_2D, texture );
	GLfloat _z = 0.0f;
	glPushMatrix();
	glTranslatef(x, y, 0);
	glRotatef(r, 0, 0, 1);
	glTranslatef(-x, -y, 0);
    glBegin( GL_QUADS );			
		glTexCoord2f(0   ,0   );	glVertex3f( x - dx		, y - dy		, _z);
		glTexCoord2f(1.0f,0   );	glVertex3f( x - dx + w	, y - dy		, _z);
		glTexCoord2f(1.0f,1.0f);	glVertex3f( x - dx + w	, y - dy + h	, _z);
		glTexCoord2f(0,   1.0f);	glVertex3f( x - dx		, y - dy + h	, _z);
    glEnd();
	glPopMatrix();
}

float viewRange = 0.5f;


void drawKover () {
	glBindTexture ( GL_TEXTURE_2D, glKoverTexture );
	float x = 0, y = 0;
	fly->GetXY( x, y );
	const float mod = 1.0f;
	x /= SCREEN_WIDTH ;
	y /= SCREEN_HEIGHT ;

	float viewRange = fly->GetZ();
	float xkl, yku;
	float xkr, ykd;
	xkl = x;
	xkr = 1.0f - xkl; 
	yku = y;
	ykd = 1.0f - yku;
	printf ( "%.3f %.3f\n", x, viewRange*mod*xkl );
	float _z = -1.0f;
	//glTranslatef( 0, 0, _z );
    glBegin( GL_QUADS );											
		glTexCoord2f(x - viewRange*mod*xkl, y - viewRange*yku );	glVertex3f( 0			, 0				, _z);
		glTexCoord2f(x + viewRange*mod*xkr, y - viewRange*yku );	glVertex3f( SCREEN_WIDTH, 0				, _z);
		glTexCoord2f(x + viewRange*mod*xkr, y + viewRange*ykd );	glVertex3f( SCREEN_WIDTH, SCREEN_HEIGHT , _z);
		glTexCoord2f(x - viewRange*mod*xkl, y + viewRange*ykd );	glVertex3f( 0			, SCREEN_HEIGHT , _z);
    glEnd();
	if ( minimapEnabled ) {
		glLineWidth(2.5); 
		glColor4f(1.0, 0.0, 0.0, 0.5f);
		glBegin(GL_LINE_LOOP);
			glVertex2f( (x - viewRange*mod*xkl) * SCREEN_WIDTH  , (y - viewRange*yku)*SCREEN_HEIGHT );
			glVertex2f( (x + viewRange*mod*xkr) * SCREEN_WIDTH  , (y - viewRange*yku)*SCREEN_HEIGHT );
			glVertex2f( (x + viewRange*mod*xkr) * SCREEN_WIDTH  , (y + viewRange*ykd)*SCREEN_HEIGHT );
			glVertex2f( (x - viewRange*mod*xkl) * SCREEN_WIDTH  , (y + viewRange*ykd)*SCREEN_HEIGHT );
		glEnd();
		glColor3f(1.0, 1.0, 1.0);
	}
}