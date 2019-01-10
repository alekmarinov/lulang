#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "obj.h"

int memory_balance = 0;

/* memory utils */

void DEFUN(d_alloc, (CALLER_ARG uint size))
	memory_balance += size;
}

void DEFUN(d_free, (CALLER_ARG uint size))
	memory_balance -= size;
}

void *DEFUN(m_new, (CALLER_ARG uint size))
	d_alloc(CALL_ARG size);
	return malloc(size);
}

void *DEFUN(m_new0, (CALLER_ARG uint size))
	d_alloc(CALL_ARG size);
	return calloc(size, 1);
}

void DEFUN(m_free, (CALLER_ARG void *p, int size))
	if (p)
	{
		free(p);
		d_free(CALL_ARG size);
	}
	else
	{
		if (size > 0)
		{
			/*
			 * FIXME: Assertion size>0 with null pointer 
			 */
		}
	}
}

Str DEFUN(s_new, (CALLER_ARG0))
	Str s = o_new(CALL_ARG0);

	s->t = STRING;
	return s;
}

Str DEFUN(s_news, (CALLER_ARG const char *s))
	Str str = s_new(CALL_ARG0);

	str->v.ptr = p_new(CALL_ARG 0);
	str->v.ptr->p = (uchar *) s;
	str->v.ptr->l = (uint)strlen(s);
	return str;
}

Obj DEFUN(n_new, (CALLER_ARG0))
	Obj o = o_new(CALL_ARG0);

	if (o)
		o->t = NUMBER;
	return o;
}

Obj DEFUN(n_newd, (CALLER_ARG double d))
	Obj o = n_new(CALL_ARG0);

	if (o)
		o->v.d = d;
	return o;
}

void DEFUN(o_free, (CALLER_ARG Obj o))
	if (o)
	{
		if (o->t == STRING)
			p_free(CALL_ARG o->v.ptr);
		m_free(CALL_ARG o, sizeof(t_Obj));
	}
}

/* compare string object with c string */
int DEFUN(s_cmps, (CALLER_ARG Str s, const char *cp))
	if (s && cp)
	{
		s_fixz(CALL_ARG s);
		return strcmp((const char *)s->v.ptr->p, cp);
	}
	return s ? 1 : cp ? -1 : 0;
}

void DEFUN(s_addc, (CALLER_ARG Str s, char c))
	if (s)
	{
		if (0 == s->v.ptr)
		{
			s->v.ptr = p_new(CALL_ARG STR_GROW_STEP);
			/*
			 * FIXME: s->v.ptr == 0 - insufficent memory 
			 */
		}

		s->v.ptr = p_resize(CALL_ARG s->v.ptr, s->v.ptr->l + 1);
		if (s->v.ptr)
			s->v.ptr->p[s->v.ptr->l++] = c;
	}
}

void DEFUN(s_fixz, (CALLER_ARG Str s))
	if (s && (!s->v.ptr || 0 != s->v.ptr->p[s->v.ptr->l - 1]))
	{
		s_addc(CALL_ARG s, 0);
	}
}

void DEFUN(s_adds, (CALLER_ARG Str s, Str s2))
	if (s && s2 && s2->v.ptr->l > 0)
	{
		if (0 == s->v.ptr)
		{
			s->v.ptr = p_new(CALL_ARG s2->v.ptr->l);
			/*
			 * FIXME: s->v.ptr == 0 - insufficent memory 
			 */
		}

		s->v.ptr = p_resize(CALL_ARG s->v.ptr, s->v.ptr->l + s2->v.ptr->l);
		if (s->v.ptr)
		{
			memcpy(s->v.ptr->p + s->v.ptr->l, s2->v.ptr->p, s2->v.ptr->l);
			s->v.ptr->l += s2->v.ptr->l;
		}
	}
}

/* converts string to number */
int DEFUN(s_tonum, (CALLER_ARG Str s, Num n))
	char *e;

	s_fixz(CALL_ARG s);
	n->v.d = strtod((char *)s->v.ptr->p, &e);
	if ((uchar *) e == s->v.ptr->p)
	{
		/*
		 * FIXME: no conversion 
		 */
		return 0;
	}
	while (isspace(*e))
		e++;
	if (*e != 0)
	{
		/*
		 * FIXME: invalid trailing character 
		 */
		return 0;
	}
	return 1;
}

/* memory utils */

Obj DEFUN(o_new, (CALLER_ARG0))
	Obj o = (Obj) m_new0(CALL_ARG sizeof(t_Obj));

	/*
	 * FIXME: o == 0 insufficent memory 
	 */
	return o;
}

Ptr DEFUN(p_new, (CALLER_ARG uint size))
	void *mem = (size > 0) ? m_new(CALL_ARG size) : 0;
	Ptr p = 0;

	if (mem || size == 0)
	{
		p = (Ptr) m_new(CALL_ARG sizeof(t_Ptr));
		p->p = (uchar *) mem;
		p->l_actual = size;
		p->l = 0;
		p->allocated = (mem) ? 1 : 0;
	}							/* FIXME: else insufficent memory */
	return p;
}

Ptr DEFUN(p_resize, (CALLER_ARG Ptr p, uint new_size))
	void *temp;

	if (0 == p->allocated || new_size > p->l_actual)
	{							/* realloc buffer */
		temp =
			(p->allocated) ? d_alloc(CALL_ARG new_size - p->l_actual),
			realloc(p->p, new_size) : m_new(CALL_ARG new_size);
		if (temp)
		{
			p->p = (uchar *) temp;
			p->l_actual = new_size;
		}
		else
		{						/* FIXME: insufficent memory */
			p_free(CALL_ARG p);
			p = 0;
		}
		p->allocated = 1;
	}
	return p;
}

void DEFUN(p_free, (CALLER_ARG Ptr p))
	if (p)
	{
		if (p->allocated && p->p && p->l_actual > 0)
		{
			m_free(CALL_ARG p->p, p->l_actual);
		}
		m_free(CALL_ARG p, sizeof(t_Ptr));
	}
}
