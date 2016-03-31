

CC=gcc
CFLAGS=-std=c99 -Wall -g

mvfl: mvfl.c
	$(CC) $(CFLAGS) mvfl.c linenoise.c -o mvfl
