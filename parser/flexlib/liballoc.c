/* liballoc - flex run-time memory allocation */

/* $Header: /home/daffy/u0/vern/flex/RCS/liballoc.c,v 1.5 93/12/01 11:58:26 vern Exp $ */

#ifdef STDC_HEADERS

#include <stdlib.h>

#else /* not STDC_HEADERS */

void *malloc();
void *realloc();
void free();

#endif /* not STDC_HEADERS */


void *yy_flex_alloc( size )
int size;
	{
	return (void *) malloc( (unsigned) size );
	}

void *yy_flex_realloc( ptr, size )
void *ptr;
int size;
	{
	return (void *) realloc( ptr, (unsigned) size );
	}

void yy_flex_free( ptr )
void *ptr;
	{
	free( ptr );
	}
