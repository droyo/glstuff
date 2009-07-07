/* Debugging macros - dbv requires __VA_ARGS__ as defined in C99 and present in 
	most popular compilers */
#include <stdarg.h>

/* dbg and dbv are printf wrappers that print to stderr and add the file and
	line number for right clicking in acme. dbg is for simple statements, dbv
	allows a variable number of arguments */
#ifdef DEBUG

#define dbg( fmt ) fprintf( stderr, "%s:%d:\t" fmt "\n", __FILE__, __LINE__ )
void
_dorf_dbg( char *file, int line, char *fmt, ... )
{
	fprintf( stderr, "%s:%d: ", file, line );
	va_list ap;
	va_start( ap, fmt );
	vfprintf( stderr, fmt, ap );
	va_end( ap );
}
#define dbv( fmt, ... ) _dorf_dbg(  __FILE__, __LINE__, fmt, __VA_ARGS__ )
#else
#define dbg( msg )
#define dbv( fmt, ... )
#endif

