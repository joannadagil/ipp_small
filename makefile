CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2

.PHONY: all clean

all: main

list.o: list.c list.h

tree.o: tree.c tree.h list.h

parser.o: parser.c parser.h

main.o: main.c parser.h tree.h list.h

main: main.o parser.o tree.o list.o

clean:
	rm -f *.o main