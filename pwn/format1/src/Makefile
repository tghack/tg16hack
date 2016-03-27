CC=gcc
CFLAGS=-Wall -Wextra -std=gnu11 -m32 -g

all: format1

format1: format1.c format.S
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -vf format1
