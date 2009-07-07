/* Read and write uncompressed 24 bit trueColor targa image files */
#include <stdio.h>
#include <stdlib.h>
#include "shorthand.h"

#define TGA_RAW 2

typedef struct {
	byte type, bpp;
	ushort w, h;
	byte *data;
	uint size;
} tga_image_t;

int tga_write( char *, tga_image_t * );
int tga_read( char *, tga_image_t * );

/* TGA stores pixels in bgra format. OpenGL needs them in rgba format */
void tga_swaprb( tga_image_t * );

/* Read the image data into tga_image_t struct. 1 on success, 0 on failure.
	mallocs enough memory to read the data */
int
tga_read( char *filename, tga_image_t *image )
{
	FILE *file;
	byte skip;

	chk(( file = fopen( filename, "r" )) == NULL, "Could not open file for reading\n");
	fread( &skip, sizeof(byte), 1, file );
	
	fseek( file, 2, SEEK_SET );
	fread( &image->type, sizeof(byte), 1, file );
	
	fseek( file, 12, SEEK_SET );
	fread( &image->w, sizeof(ushort), 1, file );
	
	fseek( file, 14, SEEK_SET );
	fread( &image->h, sizeof(ushort), 1, file );
	
	fseek( file, 16, SEEK_SET );
	fread( &image->bpp, sizeof(byte), 1, file );
	image->size = image->w * image->h * ( image->bpp / 8 );

	chk(( image->type != TGA_RAW ), "Only uncompressed trueColor tga files are accepted.\n");
	chk(( image->size <= 0 ), "Malformed tga file: improper size\n");

	fseek( file, skip + 18, SEEK_SET );
	/* Allocate memory for the image content */
	image->data = (byte *) malloc( image->size );

	chk( image->data == NULL, "Could not allocate memory for image\n" );
	chk( fread( image->data, sizeof(byte), image->size, file) < image->size, "Image data is missing or corrupted\n" );

	fclose( file );
	return TRUE;
}

/* Save a tga file at filename. Return 1 on success, 0 if an error occurs */
int
tga_write( char * filename, tga_image_t *image )
{
	int i;
	FILE *file;
	byte h[18];

	chk(( file = fopen( filename, "w" )) == NULL, "Could not open file for writing\n" );

	/* Create the file header */
	for ( i = 0; i < 18; i ++ )
		h[i] = 0;

	h[2] = image->type;
	h[12] = image->w & 0x00FF;
	h[13] = ( image->w & 0xFF00 ) >> 8;
	h[14] = image->h & 0x00FF;
	h[15] = ( image->h & 0xFF00 ) >> 8;
	h[16] = image->bpp;

	fwrite( h, sizeof(byte), 18, file );
	fwrite( image->data, sizeof(byte), image->size, file );
	fclose( file );
	return TRUE;
}

void
tga_swaprb( tga_image_t *tga )
{
	int i;
	for( i = 0; i < tga-> size; i += ( tga->bpp / 8 ))
		tga->data[i] ^= tga->data[i+2] ^=	tga->data[i] ^= tga->data[i+2];
}

