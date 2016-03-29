CC=gcc
CFLAGS=-Wall -Wextra -std=gnu11
LFLAGS=-lnet

.PHONY: all clean
all: evil

evil: evil.c
	$(CC) $(CFLAGS) $^ -o $@ $(LFLAGS)
	sudo setcap CAP_NET_RAW=ep $@

clean:
	rm -vf evil

