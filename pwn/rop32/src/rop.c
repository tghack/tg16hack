#define _GNU_SOURCE
#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include <limits.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <link.h>
#include <signal.h>

#define __unused __attribute__((unused))
#define __noinline __attribute__((noinline))

struct addr_info {
	void *base;
	size_t size;
};

static inline void *get_func(const char *name)
{
	void *f;

	f = dlsym(RTLD_NEXT, name);
	if (!f) {
		fprintf(stderr, "[-] Couldn't find function %s :(\n", name);
		return NULL;
	}

	return f;
}

static inline void print_func_addr(void)
{
	void *f;
	/* TODO: max length of name? */
	char name[256] = { 0 };

	printf("Function name: ");
	if (!fgets(name, sizeof(name), stdin))
		return;

	name[strcspn(name, "\n")] = '\0';

	f = get_func(name);
	if (f)
		printf("[+] Address of %s: %p\n", name, f);
}

static __noinline void insert_rop(void)
{
	ssize_t ret;
	char rop_chain[1024] = { 0 };
	void *frame = __builtin_frame_address(0);

	printf("Insert rop chain, max %zu bytes: ", sizeof(rop_chain));

	ret = read(STDIN_FILENO, rop_chain, sizeof(rop_chain));
	if (ret == -1) {
		perror("read()");
		return;
	}

	memcpy((char *)frame + 4, rop_chain, ret);
}

/* round up n to nearest multiple of size */
#define round_up(n, size) (((size) - ((n) % (size))) + (n))

static int callback(struct dl_phdr_info *info, size_t __unused size, void *data)
{
	if (!strstr(info->dlpi_name, "libc"))
		return 0;

	assert(data);
	struct addr_info *addr = (struct addr_info *)data;
	addr->base = (void *)info->dlpi_addr;
	uint64_t mem_size = 0;

	long page_size = sysconf(_SC_PAGESIZE);

	for (int i = 0; i < info->dlpi_phnum; i++) {
		/*
		 * XXX: we pick the first and best PT_LOAD section,
		 *      not sure if it always works	
		 */
		if (info->dlpi_phdr[i].p_type != PT_LOAD)
			continue;
		mem_size = round_up(info->dlpi_phdr[i].p_memsz, page_size);
		break;
	}

	addr->size = mem_size;

	return 0;
}

static void timer_handler(int __unused signum)
{
	fprintf(stderr, "Bye, bye!\n");
	exit(EXIT_SUCCESS);
}

static void search_string(void *start, size_t size)
{
	char *found;
	char str[256] = { 0 };

	printf("String (%zu bytes max): ", sizeof(str));
	if (!fgets(str, sizeof(str), stdin))
		return;

	str[strcspn(str, "\n")] = '\0';

	printf("[+] searcing for %s...\n", str);

	found = memmem(start, size, str, strlen(str));
	if (!found)
		printf("[-] couldn't find \"%s\" :(\n", str);
	else
		printf("[+] found \"%s\" at %p!\n", str, found);
}

int main(void)
{
	int choice;
	char buf[15] = { 0 };
	struct sigaction sa;
	struct addr_info info = { 0 };

	memset(&sa, 0, sizeof(sa));

	sa.sa_handler = timer_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;

	if (sigaction(SIGALRM, &sa, NULL) == -1) {
		perror("sigaction()");
		exit(EXIT_FAILURE);
	}

	alarm(60);

	setbuf(stdin, NULL);
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);

	dl_iterate_phdr(callback, &info);

	for (;;) {
		printf("ROP helper 13.37\n");
		printf("1. print libc address\n");
		printf("2. print function address\n");
		printf("3. insert ROP chain\n");
		printf("4. search for string\n");
		printf("5. quit\n");
		printf("Choice: ");

		if (!fgets(buf, sizeof(buf), stdin))
			break;

		choice = atoi(buf);

		switch (choice) {
		case 1:
			printf("[+] libc start address: %p\n", info.base);
			break;
		case 2:
			print_func_addr();
			break;
		case 3:
			insert_rop();
			break;
		case 4:
			search_string(info.base, info.size);
			break;
		case 5:
			printf("Bye!\n");
			goto out;
		default:
			printf("Invalid option!\n");
		}

		printf("\n");
	}

out:
	return 0;
}
