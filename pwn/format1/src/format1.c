#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

extern void dritt(const char *name);

int main(void)
{
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);

	printf("Construct the flag like this:\n");
	printf("1. print 2nd parameter on stack as a string\n");
	printf("2. print 4th parameter on stack as a hexadecminal number\n");
	printf("3. print 1st parameter on stack as a string\n");
	printf("Name: ");

	char name[64] = { 0 };
	if (!fgets(name, sizeof(name), stdin)) {
		perror("fgets()");
		exit(EXIT_FAILURE);
	}

	printf("Hello, ");
	dritt(name);
	
	return 0;
}
