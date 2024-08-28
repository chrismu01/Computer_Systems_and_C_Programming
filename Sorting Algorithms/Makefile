SHELL := /bin/sh
CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra

all: sorting

sorting: sorting.o shell.o set.o batcher.o heap.o quick.o stats.o
	$(CC) -o sorting sorting.o shell.o set.o batcher.o heap.o quick.o stats.o

batcher: batcher.o set.o
	$(CC) -o batcher batcher.o set.o

shell: shell.o set.o
	$(CC) -o shell shell.o set.o

heap: heap.o set.o
	$(CC) -o heap heap.o set.o

quick: quick.o set.o
	$(CC) -o quick quick.o set.o

stats: stats.o set.o
	$(CC) -o stats stats.o

clean:
	rm -f sorting batcher shell heap quick stats *.o

format:
	clang-format -i -style=file *.[ch]


