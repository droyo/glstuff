/* TODO - think of a  sane coordinate system */
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#include <dorf_shorthand.h>

int screen_width = 640;
int screen_height = 480;
int fullscreen = FALSE;

void draw_ridge( GLfloat *ridge );
void event_loop(void);
void handle_keypress( SDL_KeyboardEvent );
void handle_mousepress( SDL_MouseButtonEvent );
void handle_mousemotion( SDL_MouseMotionEvent );
void make_ridge( GLfloat roughness, int iter );
void generate_ridge( GLfloat *ridge, int start, int end, int iterations, GLfloat range, GLfloat roughness, GLfloat previous );

GLfloat randf( GLfloat min, GLfloat max );

GLfloat ridge_roughness;
int ridge_iterations;

GLfloat
randf( GLfloat min, GLfloat max )
{
	GLfloat f = (GLfloat) rand() / RAND_MAX ;
	
	return min + ( f * ( max - min ));
}


void
event_loop( void )
{
	int done = FALSE;
	SDL_Event event;
	int ticks = SDL_GetTicks();
	
	while( !done ) {
		/* User input */
		while( SDL_PollEvent( &event )) {
			switch( event.type ) {
				case SDL_QUIT :
					done = TRUE;
					break;
				case SDL_KEYDOWN:
					if ( event.key.keysym.sym == SDLK_ESCAPE )
						done = TRUE;
					else {
						make_ridge( ridge_roughness, ridge_iterations );
						handle_keypress( event.key );
					}
					break;
				case SDL_KEYUP:
					handle_keypress( event.key );
					break;
				case SDL_MOUSEBUTTONDOWN:
					handle_mousepress( event.button );
					break;
				case SDL_MOUSEBUTTONUP:
					handle_mousepress( event.button );
					break;
				case SDL_MOUSEMOTION:
					handle_mousemotion( event.motion );
					break;
				default:
					break;
			}
		}

		/* Drawing */
		/* bouncing_ball( SDL_GetTicks() - ticks); */
		ticks = SDL_GetTicks();
	}
}

void
handle_mousepress ( SDL_MouseButtonEvent button )
{
	return;
}

void
handle_keypress( SDL_KeyboardEvent key )
{
	return;
}

void
handle_mousemotion( SDL_MouseMotionEvent motion )
{
	return;
}

void
make_ridge( GLfloat roughness, int iter )
{
	int i;
	GLfloat *ridge;
	if ( (ridge = malloc ( sizeof(GLfloat) * screen_width)) == NULL ) {
		fprintf( stderr, "Memory allocation error\n" );
		exit(1);
	}
	for(i = 0; i < screen_width; i++ )
		ridge[i] = 0;
	generate_ridge( ridge, 0, screen_width, iter, (GLfloat) screen_height / 2, roughness, 0 );
	free( ridge );
}

/* create a ridge using midpoint displacement */
void
generate_ridge( GLfloat *ridge, int start, int end, int iterations, 
					GLfloat range, GLfloat roughness, GLfloat previous )
{
	if( iterations == 0 ) return;
	int middle = ( start + end ) / 2;
	GLfloat displacement = randf( 0 - ( range / 2 ), range / 2 );
	GLfloat left_midpoint_height, right_midpoint_height;

	ridge[middle] = previous + displacement;
	left_midpoint_height = ( ridge[middle] + ridge[start] ) / 2.0f;
	right_midpoint_height = ( ridge[middle] + ridge[end] ) / 2.0f;
	draw_ridge( ridge );
	generate_ridge( ridge, start, middle, iterations - 1, 
						range * (pow(2, -roughness)), roughness, left_midpoint_height );
	generate_ridge( ridge, middle, end, iterations - 1, 
						range * (pow(2, -roughness)), roughness, right_midpoint_height );
}

void
draw_ridge( GLfloat *ridge )
{
	int i;
	glClear( GL_COLOR_BUFFER_BIT );
	glBegin( GL_LINE_STRIP );
		for(i = 0; i < screen_width; i++ ) {
			if( ridge[i] != 0.0 ) glVertex2f( (GLfloat) i, ridge[i] );
		}
	glEnd();
	SDL_GL_SwapBuffers();
}

int
main( int argc, char **argv )
{
	srand( time( 0 ));
	ridge_roughness = 1.0;
	ridge_iterations = 6;
	if( argc > 1 ) ridge_roughness = strtof( argv[1], NULL );
	if( argc > 2 ) ridge_iterations = strtol( argv[2], NULL, 0 );

	if ( dorf_video_init() == FALSE ) {
		fprintf( stderr, "Error initializing video\n" );
		exit(1);
	}

	make_ridge( ridge_roughness, ridge_iterations );
	event_loop();
	
	return 0;
}
