#include <stdio.h>
#include <stdlib.h>

#define FLAG "TG16{overWrit3_that_shit_y0}"

static unsigned long num = 0xdeadbeef;

int main(void)
{
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);

	char buf[64] = { 0 };
	printf("Change 0x%lx (at %p) to 0xabadcafe\n", num, &num);

	printf("Enter a nice message: ");
	if (!fgets(buf, sizeof(buf) - 1, stdin)) {
		perror("fgets()");
		exit(EXIT_FAILURE);
	}

	printf("You entered: \n");
	printf(buf);

	if (num == 0xabadcafe) {
		printf("Success! The flag is: %s\n", FLAG);
	} else {
		printf("Sorry, no flag for you :(\n");
		printf("Value: %lx\n", num);
	}
	
	return 0;
}
