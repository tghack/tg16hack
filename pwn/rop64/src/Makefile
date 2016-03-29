CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -std=gnu11
LFLAGS=-ldl

.PHONY: all clean

all: rop

rop: rop64.c
	$(CC) $(CFLAGS) $(LFLAGS) $^ -o $@

clean:
	rm -vf rop
