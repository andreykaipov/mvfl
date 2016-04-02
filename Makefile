
CC ?= gcc
CFLAGS = -std=gnu99 -Wall -g
LIBFLAGS = -lm

SRCDIR = src/
LIBDIR = lib/
OBJDIR = obj/

SRCS = $(shell find $(SRCDIR) $(LIBDIR) -name "*.c")
OBJS = $(patsubst %.c,$(OBJDIR)%.o,$(SRCS))

MAIN = mvfl

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFLAGS) -o $(MAIN)

$(OBJS): $(OBJDIR)%.o: %.c | mkdirs
	$(CC) $(CFLAGS) -c $< -o $@

mkdirs:
	$(shell mkdir -p $(dir $(OBJS)))

clean:
	rm -rf $(OBJDIR) $(MAIN)

