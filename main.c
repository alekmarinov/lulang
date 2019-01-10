
#include "scn.h"

extern int memory_balance;

int main(int argc, char* argv[])
{
	static struct _FUNC_STUB _this_func={"main", 0};

	int tok;
	FILE *file = fopen(argv[1], "r");
	Scn scn = scn_new(CALL_ARG file, argv[1]);

	while ((tok = scn_next_tok(CALL_ARG scn)) != TOK_EOT)
	{
		printf("%s ", scn_tok2str(CALL_ARG tok));
		if (tok == TOK_NUMBER)
			printf("%4.6f\n", scn->semantic->v.d);
		else if (tok == TOK_IDENT || tok == TOK_STRING)
			printf("%s\n", scn->semantic->v.ptr->p);
	}

	scn_free(CALL_ARG scn);

	printf("memory leak = %d\n", memory_balance);
	fclose(file);
	return 0;
}
