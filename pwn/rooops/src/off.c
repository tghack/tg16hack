#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <limits.h>
#include <inttypes.h>
#include <link.h>
#include <string.h>

struct addr_info {
	void *libc_base;
};

static int callback(struct dl_phdr_info *info, size_t size, void *data)
{
	(void)size;

	/* FIXME: find a less dirty way? */
	if (!strstr(info->dlpi_name, "libc"))
		return 0;

	struct addr_info *a = (struct addr_info *)data;
	a->libc_base = (void *)info->dlpi_addr;

	return 0;
}

int main(void)
{
	void *addr = mmap(NULL, 4096, PROT_READ | PROT_WRITE,
			  MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (addr == MAP_FAILED) {
		perror("mmap()");
		exit(EXIT_FAILURE);
	}

	struct addr_info info = { 0 };
	dl_iterate_phdr(callback, &info);

	printf("libc base: %p\n", info.libc_base);
	printf("offset: %p\n", (void *)((uint64_t)addr -
					(uint64_t)info.libc_base));

	return 0;
}
