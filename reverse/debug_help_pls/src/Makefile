CC=gcc
CFLAGS=-Wall -Wextra -std=gnu99

.PHONY: all clean

all: printflag

printflag: printflag.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f printflag

