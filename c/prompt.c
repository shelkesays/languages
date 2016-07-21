#include <stdio.h>
#include <stdlib.h>

#include <editline/readline.h>
#include <editline/history.h>

/* Declare a buffer for user input of size 2048 */

int main(int argc, char** argv) {
	
	/* Print version and Exit Information  */
	puts("Lispy Version 0.0.0.0.1");
	puts("Press Ctrl+c to Exit\n");

	/* In never ending loop */
	while (1) {
		
		/* Output our prompt and get input */
		char* input = readline("lispy> ");
		
		/* Add input to history */
		add_history(input);

		/* Echo input back to user */
		printf("No you're a %s\n", input);

		/* Fre retrieved input */
		free(input);
	}
	
	return 0;
}
