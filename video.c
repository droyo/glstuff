/* Copied mostly from gpwiki.org and various tutorials */
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>

/* functions for using SDL to get an openGL window */
int game_video_init( void );
SDL_Surface *game_screen;

/* Default screen size 640x480 windowed */
int game_screen_width;
int game_screen_height;
int game_fullscreen;


static int game_opengl_setup( int width, int height );

/* Create an openGL window. 1 on success. 0 on failure */
int
game_video_init( void )
{
	int depth, flags;
	const SDL_VideoInfo *info;

	if ( SDL_Init( SDL_INIT_VIDEO ) < 0) {
		fprintf( stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
		return FALSE;
	}
	atexit( SDL_Quit );

	info = SDL_GetVideoInfo();
	depth = info->vfmt->BitsPerPixel;

	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, TRUE );

	/* Use available hw acceleration and doublebuffering etc */
	flags = SDL_OPENGL | SDL_HWPALETTE | SDL_GL_DOUBLEBUFFER | \
		(( info->hw_available ) ? SDL_HWSURFACE : SDL_SWSURFACE ) | \
		(( info->blit_hw ) ? SDL_HWACCEL : 0 | game_fullscreen ? SDL_FULLSCREEN : 0 );

	if ( !( game_screen = SDL_SetVideoMode( game_screen_width, game_screen_height, depth, flags ))) {
		fprintf( stderr, "Unable to set video mode: %s\n", SDL_GetError());
		return FALSE;
	}

	return game_opengl_setup( game_screen_width, game_screen_height );
}

/* OpenGL Voodoo magic */
int
game_opengl_setup( int width, int height )
{
	glClearColor( 0.0, 0.0, 0.0, 0.0 );

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	/* cartesian coordinates */
	gluOrtho2D( 0.0, game_screen_width, -(game_screen_height / 2), game_screen_height / 2 );
	glMatrixMode(GL_MODELVIEW);

	glDisable(GL_DEPTH_TEST);
	return TRUE;
}
