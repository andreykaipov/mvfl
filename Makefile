
# CC = gcc
CFLAGS = -std=gnu99 -Wall -g
LIBFLAGS = -lm

SRCS = $(shell find . -name "*.c" | cut -b 3-)

OBJDIR = objs
OBJS = $(SRCS:%.c=$(OBJDIR)/%.o)

MAIN = mvfl

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFLAGS) -o $(MAIN)

$(OBJS): $(OBJDIR)/%.o: %.c
	$(shell mkdir -p $(dir $@))
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(MAIN)

