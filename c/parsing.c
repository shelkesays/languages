#include "mpc.h"

/* If we are compiling on Windows compile these functions */
#ifdef _WIN32

static char buffer[2048];

/* Fake readline function */
char* readline(char* prompt) {
	fputs(prompt, stdout);
	fgets(buffer, 2048, stdin);
	char* cpy = malloc(strlen(buffer)+1);
	strcpy(cpy, buffer);
	cpy[strlen(cpy)-1] = '\0';
	return cpy;
}

/* Fake add history function */
void add_history(char* unused) {}

/* otherwise include the editline headers */
#else
#include <editline/readline.h>
#include <editline/history.h>
#endif

int main(int argc, char** argv) {
	
	/* Create some parser */
	mpc_parser_t* Number	= mpc_new("number");
	mpc_parser_t* Operator	= mpc_new("operator");
	mpc_parser_t* Expr	= mpc_new("expr");
	mpc_parser_t* Ark	= mpc_new("ark");

	/* Define them with the following language */
	mpca_lang(MPCA_LANG_DEFAULT,
		"										\
	   		number		:	/-?[0-9]+[.]?[0-9]*/ ;				\
	   		operator	:	'+' | '-' | '*' | '/' | '%' | \"add\" | \"sub\" | \"mul\" | \"div\";		\
	   		expr		:	<number> | '(' <operator> <expr>+ ')' ;		\
	   		ark		:	/^/ <operator> <expr>+ /$/ ;			\
		",
		Number, Operator, Expr, Ark);

	/* Print version and Exit Information  */
	puts("Ark Version 0.0.0.0.2");
	puts("Press Ctrl+c to Exit\n");

	/* In never ending loop */
	while (1) {
		
		/* Now in either case readline will be correctly defined  */
		char* input = readline("ark> ");
		add_history(input);

		/* Attempt to parse the user input */
		mpc_result_t r;
		if(mpc_parse("<stdin>", input, Ark, &r)) {
			/* on success Print the LAST */
			mpc_ast_print(r.output);
			mpc_ast_delete(r.output);
		} else {
			/* Otherwise Print the Error */
			mpc_err_print(r.error);
			mpc_err_delete(r.error);
		}
		
		free(input);
	}
	
	/* Undefine and Delete our Parsers */
	mpc_cleanup(4, Number, Operator, Expr, Ark);

	return 0;
}
