CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -std=gnu11 -O1 -fstack-protector -D_FORTIFY_SOURCE=2 -Wno-implicit-function-declaration -Wl,-Tlol.ld

.PHONY: all clean

all: leak

leak: leak.c
	$(CC) $(CFLAGS) $^ -o $@ -DFLAG=\"TG16{fortified_info_leak_ftw}\"

task: leak.c
	$(CC) $(CFLAGS) $^ -o $@ -DFLAG=\"TG16{flag_goes_here}\"

clean:
	rm -vf leak task
