
MAIN = mvfl

CC ?= gcc
CFLAGS = -std=gnu99 -Wall -g
LIBFLAGS = -lm

SRCDIR = src/
LIBDIR = lib/
OBJDIR = obj/

SRCS = $(shell find $(SRCDIR) $(LIBDIR) -name "*.c")
HDRS = $(shell find $(SRCDIR) $(LIBDIR) -name "*.h")
OBJS = $(patsubst %.c,$(OBJDIR)%.o,$(SRCS))

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFLAGS) -o $(MAIN)

# .o files depend on identically named .c and .h files.
$(OBJS): $(OBJDIR)%.o: %.c %.h | mkdirs
	$(CC) $(CFLAGS) -c $< -o $@

mkdirs:
	$(shell mkdir -p $(dir $(OBJS)))

# Silently create any dependencies that fall through to DEFAULT.
# For example, not every .c file will a corresponding .h header.
.DEFAULT:
	$(shell touch $@)
  
# Delete the empty dependencies too.
clean:
	rm -rf $(OBJDIR) $(MAIN)*
	find . -type f -size 0 -delete

