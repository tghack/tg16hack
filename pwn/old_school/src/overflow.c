#include <stdio.h>
#include <inttypes.h>

int main(void)
{
	char buf[64];

	void *frame = __builtin_frame_address(0);
	void *ret_addr = (char *)frame + 4;

	fprintf(stderr, "buf: %p\n", (void *)buf);
#if 0
	fprintf(stderr, "frame: %p\n", frame);
	fprintf(stderr, "ret: %p\n", *(void **)((char *)ret_addr));
	fprintf(stderr, "ret: %p\n", __builtin_return_address(0));
#endif

	size_t len = (size_t)((char *)ret_addr - buf);
	fprintf(stderr, "ret offset: %zu\n", len);

#if 0
	fprintf(stderr, "frame: %p\n", frame);
	fprintf(stderr, "ret: %p\n", ret_addr);
	void *lol = *(void **)ret_addr;
	fprintf(stderr, "retval?: %p\n", (void *)lol);
	fprintf(stderr, "retval: %p\n", __builtin_return_address(0));
	fprintf(stderr, "buf + len: %p\n", (void *)(buf + len));
#endif

	fprintf(stderr, "Input:");

	fgets(buf, 512, stdin);

	return 0;
}
