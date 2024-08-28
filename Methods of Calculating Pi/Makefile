SHELL := /bin/sh
CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra

all: mathlib-test

mathlib-test: mathlib-test.o e.o madhava.o euler.o bbp.o viete.o newton.o
	$(CC) -o mathlib-test mathlib-test.o e.o madhava.o euler.o bbp.o viete.o newton.o -lm

e: e.o
	$(CC) -o e e.o

euler: euler.o
	$(CC) -o euler euler.o newton.o

bbp: bbp.o
	$(CC) -o bbp bbp.o

madhava: madhava.o
	$(CC) -o madhava madhava.o

viete: viete.o
	$(CC) -o viete viete.o

newton: newton.o
	$(CC) -o newton newton.o
clean:
	rm -f mathlib-test bbp e euler madhava viete newton *.o

format:
	clang-format -i -style=file *.[ch]
