#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>
#include "word.h"
#include "io.h"
#include "endian.h"
#include "trie.h"
#include "code.h"

void get_help(void) {
    fprintf(stderr, "SYNOPSIS\n"
                    "   Decompresses files with the LZ78 decompression algorithm.\n"
                    "   Used with files compressed with the corresponding encoder.\n"
                    "\n"
                    "USAGE\n"
                    "   ./test_decode [-vh] [-i input] [-o output]\n"
                    "\n"
                    "OPTIONS\n"
                    "   -v          Display decompression statistics\n"
                    "   -i input    Specify input to decompress (stdin by default)\n"
                    "   -o output   Specify output of decompressed input (stdout by default)\n"
                    "   -h          Display program usage\n");
}

#define OPTIONS "vi:o:"
int main(int argc, char **argv) {
    bool verbose_checker = false;
    int infile = fileno(stdin);
    int outfile = fileno(stdout);
    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'v': //verbose
            verbose_checker = true;
            break;

        case 'i': //infile
            infile = open(optarg, O_RDONLY);
            if (infile == -1) { //prints an error message if infile is NULL
                perror("error");
                return EXIT_FAILURE;
            }
            break;

        case 'o': //outfile
            outfile = open(optarg, O_RDONLY);
            if (outfile == -1) { //prints an error message if outfile is NULL
                perror("error");
                return EXIT_FAILURE;
            }
            break;

        case 'h': //get help case
            get_help();
            return 0;

        default: get_help(); return 0;
        } //closing of the switch cases
    } //closing of the while loop
    /*
	Word *table = wt_create();
	int curr_sym = 0;
	int curr_code = 0;
	int next_code = START_CODE;
	while ((read_pair(infile, &curr_code, &curr_sym, BIT_LENGTH(next_code))) == true) {
		table[text_code] = word_append_sym(table[curr_code], curr_sym);
		write_word(outfile,table[next_code]);
		next_code = next_code + 1;
		if (next_code == MAX_CODE) {
			wt_reset(table);
			next_code = START_CODE;
		}
	}	
	flush_words(outfile);
	*/
    if (verbose_checker == true) {
        return 0;
    }
    return 0;
} //closing of main
