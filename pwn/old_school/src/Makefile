CC=clang # fucking gcc stack protection shit
CFLAGS=-Wall -Wextra -Wpedantic -std=gnu11 -O0 -m32 -fno-stack-protector -z execstack

.PHONY: all clean
all: overflow

overflow: overflow.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -vf overflow
