CC=gcc
CFLAGS=-I.
SRCS = $(wildcard *.c)

PROGS = $(patsubst %.c,%,$(SRCS))

all: $(PROGS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $< -lm

clean: 
	rm -f $(PROGS)
