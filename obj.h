#ifndef __OBJ_H
#define __OBJ_H

#include "lim.h"
#include "dbg.h"

/**
 * defines the atom types
 */
typedef enum
{
	UNDEF = 0,
	STRING,
	NUMBER
} o_type;

/**
 * memory structure used for defining objects
 */
typedef struct
{
	uchar *p;					/* memory pointer */
	uint l;						/* expected memory size */
	uint l_actual;				/* actual memory size */
	ubool allocated;			/* true if p must be freed when destroying object */
} t_Ptr, *Ptr;

/**
 * Base object type
 */
typedef struct
{
	union
	{
		double d;				/* store double value */
		Ptr ptr;				/* store string value */
	} v;
	o_type t;					/* string or number identifier */
} t_Obj, *Obj;

/**
 * String type
 */
typedef Obj Str;

/**
 * Number type
 */
typedef Obj Num;

/**
 * Stack structure
 */
typedef struct
{
	int top;
	Obj items;
} t_Stk, *Stk;

Ptr p_new	(CALLER_ARG uint size);	/* create new pointer object */
Ptr p_resize(CALLER_ARG Ptr p, uint new_size);	/* resize pointer object */
void p_free	(CALLER_ARG Ptr p);	/* free pointer object */
void *m_new	(CALLER_ARG uint size);	/* allocates memory */
void m_free	(CALLER_ARG void *p, int size);	/* free allocated memory */
Obj o_new	(CALLER_ARG0);		/* create object */
Str s_new	(CALLER_ARG0);		/* create string */
Str s_news	(CALLER_ARG const char *s);	/* create initialized string */
Num n_new	(CALLER_ARG0);		/* create number */
Num n_newd	(CALLER_ARG double d);	/* create initialized number */
void o_free	(CALLER_ARG Obj o);	/* free object */

void s_fixz	(CALLER_ARG Str s);	/* make Str object zero terminated */
int s_cmps	(CALLER_ARG Str s, const char *c);	/* compare string object with c string */
void s_addc	(CALLER_ARG Str s, char c);	/* add char to string */
void s_adds	(CALLER_ARG Str s, Str s2);	/* add string to string */
int s_tonum	(CALLER_ARG Str s, Num n);	/* converts string to number */

#endif
