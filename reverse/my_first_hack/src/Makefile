CC=gcc
CFLAGS= -std=gnu99

.PHONY: all clean

all: myfirsthack

myfirsthack: strings.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f myfirsthack

