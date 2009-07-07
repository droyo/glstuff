/* This is my attempt at implementing the world geometry
	system used in the game LOVE by Eskil Steenburg as
	described here:
	http://iloapp.quelsolaar.com/blog/news?Home&post=45

	It uses a lot of arithmetic and integers rather than
	pointers, so it may be harder to understand, but
	I'd like to see if it has any benefits.

	This module will take the prefix gg_ */

#include "common.h"
#include "shorthand.h"

typedef struct {
	float x, y, z;
} gg_vector_t;


int		gg_triangles_count = 0;
int		gg_quads_count = 0;
int		gg_vertices_length = 128;
gg_vector_t	gg_vertices[gg_polygons_length];
int		*gg_vertices_ref;

int *		gg_compute_neighbors( int *ref, int ref_count, float *vertex, int vertex_count );


int *
gg_compute_neighbors( int *ref, int ref_count, float *vertex, int vertex_count )
{
	int i, j;
	int *neighbors = malloc( sizeof( int ) * ref_count );

	chk_retnull(( neighbours == NULL ), "gg_compute_neighbours: mae\n");
	for( i = 0; i < neighbors_count; i++ ) { n[i] = -1; }
}

