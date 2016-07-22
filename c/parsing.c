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

/* Use operator string to see which operation to perform */
long eval_op(long x, char* op, long y) {
	if (strcmp(op, "+") == 0 || strcmp(op, "add") == 0) { return x + y; }
	if (strcmp(op, "-") == 0 || strcmp(op, "sub") == 0) { return x - y; }
	if (strcmp(op, "*") == 0 || strcmp(op, "mul") == 0) { return x * y; }
	if (strcmp(op, "/") == 0 || strcmp(op, "div") == 0) { return x / y; } 
	if (strcmp(op, "%") == 0 || strcmp(op, "mod") == 0) { return x % y; }
	if (strcmp(op, "^") == 0 || strcmp(op, "raise") == 0) { return x ^ y; }
	if (strcmp(op, "min") == 0) { return x < y ? x : y; }
	if (strcmp(op, "max") == 0) { return x > y ? x : y; }
	return 0;
}

long eval(mpc_ast_t* t) {

	/* Tagged as number return it directly. */
	if(strstr(t->tag, "number")) {
		return atoi(t->contents);
	}
	
	/* The operator is always second child. */
	char* op = t->children[1]->contents;

	/* We store the third child in `x` */
	long x = eval(t->children[2]);

	/* Iterate the remaining children and combining. */
	int i = 3;
	while(strstr(t->children[i]->tag, "expr")) {
		x = eval_op(x, op, eval(t->children[i]));
		i++;
	}

	return x;
}

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
	   		operator	:	'+' | '-' | '*' | '/' | '%' | \"add\" | \"sub\" | \"mul\" | \"div\" | \"mod\" | \"raise\" | \"min\" | \"max\" ;		\
	   		expr		:	<number> | '(' <operator> <expr>+ ')' ;		\
	   		ark		:	/^/ <operator> <expr>+ /$/ ;			\
		",
		Number, Operator, Expr, Ark);

	/* Print version and Exit Information  */
	puts("Ark Version 0.0.0.0.2");
	puts("Press Ctrl+c to Exit\n");
	puts("You can also type `exit` OR `q` OR `quit` to Exit\n");

	/* In never ending loop */
	while (1) {
		
		/* Now in either case readline will be correctly defined  */
		char* input = readline("ark> ");
		add_history(input);

		if (strcmp(input, "exit") == 0 || strcmp(input, "q") == 0 || strcmp(input, "quit") == 0) {
			exit(0);
		}

		/* Attempt to parse the user input */
		mpc_result_t r;
		if(mpc_parse("<stdin>", input, Ark, &r)) {
			
			long result = eval(r.output);
			printf("%li\n", result);
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
