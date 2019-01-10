#ifndef __SCN_H
#define __SCN_H

#include <stdio.h>
#include "obj.h"

#define FIRST_RESERVED	257

enum TOKENS
{
	/*
	 * reserved words 
	 */
	TOK_AND = FIRST_RESERVED, TK_BREAK,
	TOK_DO, TOK_ELSE, TOK_ELSEIF, TOK_END, TOK_FOR, TOK_FUNCTION,
	TOK_IF, TOK_NIL, TOK_NOT, TOK_OR, TOK_REPEAT,
	TOK_RETURN, TOK_THEN, TOK_UNTIL, TOK_WHILE,
	/*
	 * other terminal symbols 
	 */
	TOK_IDENT, TOK_EQ, TOK_GE, TOK_LE, TOK_NE, TOK_NUMBER,
	TOK_STRING, TOK_EOT
};

typedef struct
{
	int current;
	int lineno;
	FILE *file;
	Obj filename;
	uint tok;
	Obj semantic;
	Str buffer;
} t_Scn, *Scn;

Scn scn_new(CALLER_ARG FILE * infile, const char *filename);
void scn_free(CALLER_ARG Scn scn);
int scn_next_tok(CALLER_ARG Scn scn);
const char *scn_tok2str(CALLER_ARG int tok);

#endif
