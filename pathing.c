/* pplay - an environment for pathfinding experiments */
#include "common.h"
#include "pplay.h"

#define BUFSIZE 256

typdef struct {
	int type;
	float x, y, width, height, rotation;
} pplay_object_id;

typedef struct {
	pplay_object_id id;
	int intensity;
} pplay_goal_t;

typedef struct {
	pplay_object_id id;
	float motion_vector[2];
} pplay_agent_t;
enum { PPLAY_GOAL, PPLAY_AGENT };

/* All the game objects are stored in arrays that are realloc'd as needed. */
unsigned int		pplay_obstacles_count = 0;
unsigned int		pplay_obstacles_length = 32;
pplay_obstacle_t	pplay_obstacles[pplay_obstacles_length];

unsigned int		pplay_agents_count = 0;
unsigned int		pplay_agents_length = 64;
pplay_agent_t		pplay_agents[pplay_agents_length];

unsigned int		pplay_goals_count = 0;
unsigend int		pplay_goals_length = 8;
pplay_goal_t		pplay_goals[pplay_goals_length];

/* Several modes, for placing objects, typing commands, etc */
int				pplay_capture_motion = FALSE;
int				pplay_capture_clicks = FALSE;
int				pplay_capture_keys = FALSE;
int				pplay_exit_flag = FALSE;
int				pplay_draw_wireframe = FALSE;
long long			pplay_clock_ticks = 0;
char				pplay_keyboard_input[BUFSIZE];

pplay_vector_t		pplay_vector_create( float x, float y );
void				pplay_geometry_assign( pplay_geometry_t *geom, int x, int y, float width, float height );

/* Navigation mesh */
pplay_mesh_t		pplay_mesh;
void				pplay_mesh_update( void );


/* (x,y) is the location of top left corner for rect, center for circle */
obstacle_rect_t		obstacle_rect_create( int x, int y, float width, float height );
obstacle_circle_t	obstacle_circle_create( int x, int y, float diameter );
void				pplay_obstacle_destroy( pplay_obstacle * );

pplay_agent_t		pplay_agent_create( int x, int y );
void				pplay_agent_step( pplay_agent_t *, int dt );
void				pplay_agent_destroy( pplay_agent * );

pplay_goal_t		pplay_goal_create( int x, int y );
void				pplay_goal_destroy( pplay_goal * );

int				pplay_obstacles_checksize(void);
int				pplay_agents_checksize(void);
int				pplay_goals_checksize(void);

void				pplay_event_loop();

void				obstacle_circle_draw( obstacle_circle_t * );
void				obstacle_rect_draw( obstacle_rect_t * );
void				pplay_agent_draw( pplay_agent_t * );
void				pplay_goal_draw( pplay_goal_t * );
void				pplay_scene_draw( void );
void				pplay_interface_draw( void );
void				pplay_cleanup( void );

/* Memory allocation functions - if memory limit is reached realloc 133% of the current
	space */
int
pplay_obstacles_checksize( void )
{
	int increase = pplay_obstacles_count / 3;
	if( pplay_obstacles_count == pplay_obstacles_length ) {
		chk( realloc( pplay_obstacles, pplay_obstacles_length + increase ) == NULL, "MAE" );
		pplay_obstacles_length += increase;
	}
	return 0;
}

int
pplay_agents_checksize( void )
{
	int increase = pplay_agents_count / 3;
	if( pplay_agents_count == pplay_agents_length ) {
		chk( realloc( pplay_agents, pplay_agents_length + increase ) == NULL, "MAE" );
		pplay_agents_length += increase;
	}
	return 0;
}

int
pplay_goals_checksize( void )
{
	int increase = pplay_goals_count / 3;
	if( pplay_goals_count == pplay_goals_length ) {
		chk( realloc( pplay_goals, pplay_goals_length + increase ) == NULL, "MAE" );
		pplay_goals_length += increase;
	}
	return 0;
}

/* Object creation functions */
obstacle_rect_t
obstacle_rect_create( int x, int y, float width, float height )
{
	obstacle_rect_t rect;
	int index = pplay_obstacles_count;
	pplay_obstacles_count++;

	if( pplay_obstacles_checksize() ) {
		fprintf( stderr, "Couldn't reallocate memory for more obstacles\n" );
		pplay_cleanup();
		exit(1);
	}
	pplay_geometry_assign( &rect.geometry, x, y, width, height );
	rect.vertices[0] = pplay_vector_create( (float) x, (float) y );
	rect.vertices[1] = pplay_vector_create( (float) x + width, (float) y );
	rect.vertices[2] = pplay_vector_create( (float) x + width, (float) y + height );
	rect.vertices[3] = pplay_vector_create( (float) x, (float) y + height );
	
	pplay_obstacles[index].obstacle = rect;
	pplay_obstacles[index].type = PPLAY_RECT;
}

obstacle_circle_t
obstacle_circle_create( int x, int y, float diameter )
{
	obstacle_circle_t circle;
	int index = pplay_obstacles_count;

	if( pplay_obstacles_checksize() ) {
		fprintf( stderr, "Couldn't reallocate memory for more obstacles\n" );
		pplay_cleanup();
		exit(1);
	}
	pplay_geometry_assign( &circle.geometry, x, y, diameter, diameter );
	
	pplay_obstacles[index].obstacle = circle;
	pplay_obstacles[index].type = PPLAY_CIRCLE;
}

pplay_agent_t
pplay_agent_create( int x, int y )
{
	pplay_agent_t agent;
	int index = pplay_agents_count;
	const pplay_vector_t no_motion = { 0.0f, 0.0f };
	pplay_agents_count++;
	
	if( pplay_agents_checksize() ) {
		fprintf( stderr, "Couldn't reallocate memory for more agents\n" );
		pplay_cleanup();
		exit(1);
	}
	pplay_geometry_assign( &agent.geometry, x, y, 0.0f, 0.0f );
	agent.motion_vector = no_motion;
	
	pplay_agents[index] = agent;
}

pplay_goal_t
pplay_goal_create( int x, int y )
{
	pplay_goal_t goal;
	int index = pplay_goals_count;
	pplay_goals_count++;
	
	if( pplay_goals_checksize() ) {
		fprintf(stderr, "Couldn't reallocate memory for more goals\n" );
		pplay_cleanup();
		exit(1);
	}
	pplay_geometry_assign( &goal.geometry, x, y, 0.0f, 0.0f );

	pplay_goals[index] = goal;
}

/* Destruction - point the current object to the final one and decrement object counter. This works
	because we never shrink the object arrays. If we want to do that we have to copy the objects
	over explicitly */
void
pplay_obstacle_destroy( pplay_obstacle_t *obstacle )
{
	if( pplay_obstacles_count == 0 )
		return;
	else
		obstacle = &pplay_obstacles[--pplay_obstacles_count];
}

void
pplay_agent_destroy( pplay_agent_t *agent )
{
	if( pplay_agents_count == 0 )
		return;
	else
		agent = &pplay_agents[--pplay_agents_count];
}

void
pplay_goal_destroy( pplay_goal_t *goal )
{
	if( pplay_goals_count == 0 )
		return;
	else
		goal = &pplay_goals[--pplay_goals_count];
}

void
pplay_cleanup( void )
{
	free(pplay_obstacles);
	free(pplay_agents);
	free(pplay_goals);
}

void
pplay_mesh_update( void )
{
	int i;
	
}

void
pplay_event_loop( void )
{
	int dt, i;
	SDL_Event event;
	pplay_clock_ticks = SDL_GetTicks();
	
	while( !pplay_exit_flag ) {
		while( SDL_PollEvent(&event)) {
			switch( event.type ) {
				case SDL_QUIT:
					pplay_exit_flag = TRUE;
					break;
				case SDL_KEYUP:
					pplay_handle_keypress( event.key );
					break;
				case SDL_MOUSEMOTION:
					pplay_handle_mousemotion( event.motion );
					break;
				case SDL_MOUSEBUTTONDOWN:
					pplay_handle_click( event.button );
					break;
				case SDL_MOUSEBUTTONUP:
					pplay_handle_click( event.button );
					break;
			}
		}
		pplay_mesh_update();
		for(i = 0; i < pplay_agents_count; i++ ) {
			pplay_agent_step( &pplay_agents[i] );
		}
		/* Delay according to FPS here */
		pplay_scene_draw();
		pplay_interface_draw();
	}
}

/* Drawing functions */
void
pplay_interface_draw( void )
{
	
}
