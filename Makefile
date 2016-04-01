
# CC = gcc
CFLAGS = -std=gnu99 -Wall -g
LIBFLAGS = -lm

SRCS = mvfl.c linenoise.c mpc.c
OBJS = $(SRCS:.c=.o)
MAIN = mvfl

ifeq ($(OS),Windows_NT)
	TRASH = *.o mvfl.exe
else
	TRASH = *.o mvfl
endif

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS) $(LIBFLAGS)

# The following is an implicit rule, but I like it.
# $< is the name of the related file that caused this action.
.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm $(TRASH)

