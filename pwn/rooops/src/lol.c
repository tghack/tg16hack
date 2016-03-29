#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAP_SIZE 4096
#define __noinline __attribute__((noinline))

struct user {
	char name[256];
	char *buf;
	int age;
};

static __noinline void read_user_input(struct user *user, size_t len)
{
	char buf[128] = { 0 };
	printf("Enter a message: ");

	ssize_t ret = read(STDIN_FILENO, buf, len);
	if (ret <= 0) {
		perror("read()");
		exit(EXIT_FAILURE);
	}

	strncpy(user->buf, buf, ret);
}

int main(void)
{
	setvbuf(stdout, NULL, _IONBF, 0);

	struct user user = { 0 };
	user.buf = mmap(NULL, MAP_SIZE, PROT_WRITE | PROT_READ,
			MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (user.buf == MAP_FAILED) {
		perror("mmap()");
		exit(EXIT_FAILURE);
	}

#if 0
	printf("buf: %p\n", user.buf);
	printf("pid: %d\n", getpid());
#endif
	printf("Enter name: ");

	if (read(STDIN_FILENO, user.name, sizeof(user.name) + 1) <= 0) {
		perror("read()");
		exit(EXIT_FAILURE);
	}

	user.name[strcspn(user.name, "\n")] = '\0';
	printf("Welcome, %s!\n", user.name);

	read_user_input(&user, MAP_SIZE);
	printf("You entered: %s\n", user.buf);

	return 0;
}
