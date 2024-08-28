SHELL := /bin/sh
CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra

all: life

life: life.o universe.o
	$(CC) -o life life.o universe.o -lncurses

universe: universe.o
	$(CC) -o universe universe.o
clean:
	rm -f life universe *.o

format:
	clang-format -i -style=file *.[ch]

