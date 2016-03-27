CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -std=gnu11 -m32
LFLAGS=-ldl

.PHONY: all clean

all: rop

rop: rop.c
	$(CC) $(CFLAGS) $(LFLAGS) $^ -o $@

clean:
	rm -vf rop
