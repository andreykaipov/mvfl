
# CC = gcc
CFLAGS = -std=c99 -Wall -g
LIBFLAGS = -lm

SRCS = mvfl.c linenoise.c mpc.c
OBJS = $(SRCS:.c=.o)
MAIN = mvfl

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS) $(LIBFLAGS)

# The following is an implicit rule, but I like it.
# $< is the name of the related file that caused this action.
.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm *.o *.exe
