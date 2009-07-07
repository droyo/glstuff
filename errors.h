/* These macros replace the common C error-checking idioms. I don't
	know how useful these actually are and am considering removing them,
  	as C programmers will be more familiar with the idiomatic statements */
#include <stdio.h>

/* See ../errors.awk for error codes */
#define chk(test, msg) do { if(test) { fprintf(stderr, "%s\n", msg); return FALSE; }} while (0)
#define chk_exit(test, msg) do { if(test) { fprintf(stderr, "%s\n", msg); exit(1); }} while (0)
#define chk_ret1(test, msg) do { if(test) { fprintf(stderr, "%s\n", msg); return -1; } while (0)
#define chk_retnull(test, msg) do { if(test) { fprintf(stderr, "%s\n", msg); return NULL; }} while (0)
#define chk_retval(test, msg, val) do { if(test) { fprintf(stderr, "%s\n", msg); return val; }} while (0)
