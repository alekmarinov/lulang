#ifndef __HSH_H
#define __HSH_H

#include "obj.h"

typedef struct 
{
	Str *k;
	Obj *o;
} t_HBucket, Bucket;

typedef struct 
{
	Bucket *table;
	uint size;
	uint shift;
	uint collisions;
} t_Hsh, *Hsh;

Hsh h_new(CALLER_ARG int size);
Obj h_get(CALLER_ARG Hsh, Str);
void h_set(CALLER_ARG Hsh, Str, Obj);
void h_free(CALLER_ARG Hsh);
uint h_code(CALLER_ARG Str s);

#endif
