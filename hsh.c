#include "hsh.h"

uint DEFUN(h_code, (CALLER_ARG Str s))
	uint h = 0;
	uint g;
	uint l=s->v.ptr->l;
	uchar* p=s->v.ptr->p;

	for (; l > 0; l--)
	{
		h = (h << 4) + *p++;
		if ((g = h & 0xf0000000) != 0)
			h = (h ^ (g >> 24)) ^ g;
	}
	return h;
}

Hsh DEFUN(h_new, (CALLER_ARG int size))
	Hsh h = cast(Hsh, m_new(CALL_ARG sizeof(t_Hsh)));
	h->shift = 0;
	if ((size >> 16) > 0)
	{
		size = size >> 16;
		h->shift += 16;
	}
	if ((size >> 8) > 0)
	{
		size = size >> 8;
		h->shift += 8;
	}
	if ((size >> 4) > 0)
	{
		size = size >> 4;
		h->shift += 4;
	}
	if ((size >> 2) > 0)
	{
		size = size >> 2;
		h->shift += 2;
	}
	if ((size >> 1) > 0)
	{
		size = size >> 1;
		h->shift += 1;
	}
	h->size = 1 << h->shift;

	h->table=cast(Obj*, m_new(CALL_ARG h->size * sizeof(Obj)));
	h->collisions=0;
}

void DEFUN(h_set, (CALLER_ARG Hsh h, Str s, Obj o))
	uint hash_val = h_code(CALL_ARG s);
	uint probe = hash_val & (h->size - 1);
	uint increment = ((hash_val >> h->shift) << 1) + 1;

	s_fixz(s, "h_set");
	while (h->table[probe])
	{
		if ( !s_cmps (CALL_ARG h->table[probe], s.ptr->p))
			return ;

		h->collisions++;
		probe = (probe + increment) & (h->size - 1);
	}

	h->table[probe] = o;
}

Obj DEFUN(h_get, (CALLER_ARG Hsh h, Str s))
	uint hash_val = h_code(s);
	uint probe = hash_val & (h->size - 1);
	uint increment = ((hash_val >> h->shift) << 1) + 1;

	s_fixz(CALL_ARG s);
	while (h->table[probe])
	{
		if ( !s_cmps (CALL_ARG h->table[probe], s.ptr->p))
			return h->table[probe];

		probe = (probe + increment) & (h->size - 1);
	}
	return NULL;
}

void DEFUN(h_free, (CALLER_ARG Hsh h))
	m_free(CALL_ARG h->table, h->size * sizeof(Obj));
	m_free(CALL_ARG h, sizeof(t_Hsh));
}
