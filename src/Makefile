CFLAGS=-std=c11 -Wall -g -static
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

tim: $(OBJS)
				$(CC) -o tim $(OBJS) $(LDFLAGS)

$(OBJS): tim.h

test: tim
				./tim -test
				./test.sh

clean:
				rm -f tim *.o *~ tmp*

tc: test clean;
