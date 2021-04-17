PROG = fiverocks
CC = gcc
SRCS = fiverocks.c helper.c
OBJS = $(SRCS:.c=.o)
all: $(PROG)
$(PROG): $(OBJS)
	$(CC) -o $(PROG) $(OBJS)
clean:
	rm -f $(PROG) $(OBJS)
.c.o : helper.h ; $(CC) -c $*.c
