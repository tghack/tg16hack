CC=gcc
CFLAGS=-Wall -Wextra -std=gnu99

.PHONY: all clean
all: pwn

pwn: pwn.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -vf pwn
