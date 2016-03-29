#include <stdio.h>
#include <unistd.h>

#define noinline __attribute__((noinline))

static volatile const char __attribute__((__section__((".lol")))) flag[] = FLAG;

static noinline void get_user_info(void)
{
	char buf[64] = { 0 };

	printf("Name: ");

	gets(buf);

	printf("Welcome, %s! Have a wonderful day!\n", buf);
}

int main(void)
{
	setvbuf(stdout, NULL, _IONBF, 0);
	get_user_info();

	return 0;
}
