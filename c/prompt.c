#include <stdio.h>
#include <stdlib.h>

/* If we are compiling on Windows compile these functions */
#ifdef _WIN32
#include <string.h>

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
	
	/* Print version and Exit Information  */
	puts("Ark Version 0.0.0.0.1");
	puts("Press Ctrl+c to Exit\n");
	puts("This is a general purpose language, developed for educational purpose.");
	puts("Do not use it in production! :)\n");

	/* In never ending loop */
	while (1) {
		
		/* Now in either case readline will be correctly defined  */
		char* input = readline("ark> ");
		add_history(input);

		printf("%s\n", input);
		free(input);
	}
	
	return 0;
}
