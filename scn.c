
#include <stdio.h>
#include <ctype.h>
#include <malloc.h>
#include "scn.h"
#include "obj.h"

#define scn_next(scn) (scn->current = fgetc(scn->file))

static const char *const tok2str[] = {
	"and", "break",
	"do", "else", "elseif", "end", "for", "function",
	"if", "nil", "not", "or", "repeat",
	"return", "then", "until", "while",
	"<ident>", "==", ">=", "<=", "!=", "<number>",
	"<string>", "<eof>"
};

Scn DEFUN(scn_new, (CALLER_ARG FILE * infile, const char *filename))
	Scn s = (Scn) m_new(CALL_ARG sizeof(t_Scn));

	s->current = 0;
	s->lineno = 1;
	s->filename = s_news(CALL_ARG filename);
	s->file = infile;
	s->tok = TOK_EOT;
	s->semantic = o_new(CALL_ARG0);
	s->buffer = s_new(CALL_ARG0);
	scn_next(s);
	return s;
}

void DEFUN(scn_free, (CALLER_ARG Scn scn))
	o_free(CALL_ARG scn->filename);
	o_free(CALL_ARG scn->semantic);
	o_free(CALL_ARG scn->buffer);
	m_free(CALL_ARG scn, sizeof(t_Scn));
}

void DEFUN(scn_number, (CALLER_ARG Scn scn))
	s_fixz(CALL_ARG scn->buffer);
	scn->buffer->v.ptr->l = 0;
	while (isdigit(scn->current))
	{
		s_addc(CALL_ARG scn->buffer, scn->current);
		scn_next(scn);
	}
	if (scn->current == '.')
	{
		s_addc(CALL_ARG scn->buffer, scn->current);
		scn_next(scn);
		if (scn->current == '.')
		{						/* FIXME: invalid number */
		}
	}
	while (isdigit(scn->current))
	{
		s_addc(CALL_ARG scn->buffer, scn->current);
		scn_next(scn);
	}

	if (scn->semantic->t == STRING)
	{
		p_free(CALL_ARG scn->semantic->v.ptr);
	}

	scn->semantic->t = NUMBER;

	if (!s_tonum(CALL_ARG scn->buffer, scn->semantic))
	{							/* FIXME: invalid number */
	}
}

void DEFUN(scn_ident, (CALLER_ARG Scn scn))
	scn->semantic->t = STRING;
	s_fixz(CALL_ARG scn->semantic);
	scn->semantic->v.ptr->l = 0;
	do
	{
		s_addc(CALL_ARG scn->semantic, scn->current);
		scn_next(scn);
	}
	while (isalnum(scn->current) || scn->current == '_');
}

void DEFUN(scn_string, (CALLER_ARG Scn scn))
	scn->semantic->t = STRING;
	scn->semantic->v.ptr->l = 0;
	do
	{
		s_addc(CALL_ARG scn->semantic, scn->current);
		scn_next(scn);
		if (scn->current == '\n')
		{
			/* FIXME: unterminated string */
		}else
		if (scn->current == '\\')
		{
			scn_next(scn);
			s_addc(CALL_ARG scn->semantic, scn->current);
			scn_next(scn);
		}
	}
	while (scn->current != '"');
	s_fixz(CALL_ARG scn->semantic);
}

int DEFUN(scn_next_tok, (CALLER_ARG Scn scn))
	for (;;)
	{
		switch (scn->current)
		{
		case -1:
			return TOK_EOT;
		case '\n':
			{
				++scn->lineno;
				scn_next(scn);
				continue;
			}
		case '<':
			{
				scn_next(scn);
				if (scn->current != '=')
					return '<';
				else
				{
					scn_next(scn);
					return TOK_LE;
				}
			}
		case '>':
			{
				scn_next(scn);
				if (scn->current != '=')
					return '>';
				else
				{
					scn_next(scn);
					return TOK_GE;
				}
			}
		case '=':
			{
				scn_next(scn);
				if (scn->current != '=')
					return '=';
				scn_next(scn);
				return TOK_EQ;
			}
		case '!':
			{
				scn_next(scn);
				if (scn->current != '=')
					return '!';
				scn_next(scn);
				return TOK_NE;
			}
		case '"':
			{
				scn_next(scn);
				scn_string(CALL_ARG scn);
				scn_next(scn);
				return TOK_STRING;
			}
		default:
			{
				int c;

				if (isspace(scn->current))
				{
					scn_next(scn);
					continue;
				}
				if (isdigit(scn->current))
				{
					scn_number(CALL_ARG scn);
					return TOK_NUMBER;
				}
				if (isalpha(scn->current) || scn->current == '_')
				{
					int tok;

					scn_ident(CALL_ARG scn);
					for (tok = 0; tok <= TOK_WHILE - FIRST_RESERVED; tok++)
					{
						if (!s_cmps
							(CALL_ARG scn->semantic, tok2str[tok]))
							return tok + FIRST_RESERVED;
					}
					return TOK_IDENT;
				}
				if (iscntrl(scn->current))
				{				/* FIXME: invalid control char */
				}
				c = scn->current;
				scn_next(scn);
				return c;
			}
		}
	}
}

const char *DEFUN(scn_tok2str, (CALLER_ARG int tok))
	static char buf[2];

	if (tok >= FIRST_RESERVED)
	{
		tok -= FIRST_RESERVED;
		return tok2str[tok];
	}
	buf[0] = (char)tok;
	buf[1] = 0;
	return buf;
}
