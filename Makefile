CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -g

all: make

make: make.o parser.o

.PHONY: clean
clean:
	rm -f *~ *.o make
