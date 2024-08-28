SHELL := /bin/sh
CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra


all: encode decode 

encode: encode.o io.o trie.o word.o
	$(CC) -o encode encode.o io.o trie.o word.o

decode: decode.o io.o trie.o word.o
	$(CC) -o decode decode.o io.o trie.o word.o

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f io word encode decode trie *.o

format:
	clang-format -i -style=file *.[ch]
