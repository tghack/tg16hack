CC=gcc
CFLAGS=-Wall -Wextra -O0 -std=gnu11

.PHONY: all clean
all: lol off

lol: lol.c
	$(CC) $(CFLAGS) $^ -o $@

off: off.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -vf lol
