/* luacons - lua console embedded in an opengl window */
#include <stdio.h>
#include <string.h>

#include <lua.h>
#include <luaxlib.h>
#include <lualib.h>

#include <SDL/SDL.h>
#include <SDL/SDL_TTF.h>
#include <shorthand.h>
#include "opengl_functions.c"

void	event_loop( void );
void	handle_key( SDL_Key * );
int	init( void );
void	cleanup( void );

int	luacons_init( void );
int	luacons_insert( char );
void	luacons_destroy( void );
void	luacons_move( int );
void	luacons_del( int );
void	luacons_send( void );
void	luacons_draw( void );

#define MAXLINE 256
#define LINECOUNT 50

char luacons_cmd[MAXLINE];
char	lua_output[LINECOUNT][MAXLINE];
int	lua_output_top = 0;
int	lua_output_length = 0;

TTF_Font	*default_font;
int	default_font_ptsize = 12;
int	luatext_line_count = 0;

int
main( int argc, char **argv )
{
	chk_exit( !init(), "Error initializing" );
	event_loop();
	
	return 0;
}

void
event_loop( void )
{
	int exit = FALSE;
	SDL_Event event;
	
	while ( !exit ) {
		while ( SDL_PollEvent( &event )) {
			switch ( event.type ) {
				case SDL_KEYDOWN:
					handle_key( event.key );
					break;
				
				case SDL_QUIT:
					exit = TRUE;
					break;
			}
		}
		/* Drawing stuff here */
	}
}

void
handle_key( SDL_Key *key )
{
	switch( key->keysym.sym ) {
		
		case SDLK_ESCAPE:
			SDL_PushEvent( SDL_QUIT );
			break;

		case   
}
