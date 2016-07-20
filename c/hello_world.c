#include <stdio.h>

int main(int argc, char** argv) {
	//for (int i = 0; i < 5; i++) {
	//int i = 0;
	//while(i < 5) {
	//	puts("Hi, world!");
	//	i++;
	//}
	//}
	loop(10)
	
	return 0;
}

int loop(int n) {
	for(int count = 0; count < n; n++) {
		puts("Hello, world!");
	}
	return 0;
}
