#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>
#include "io.h"
#include "endian.h"
#include "trie.h"
#include "word.h"
#include "code.h"

//help from lev
//bit length function: returns the bit length
unsigned int BIT_LEN(unsigned int number) {
    unsigned int count = 0;
    while (number) {
        count++;
        number >>= 1; //bitwise right shift by 1
    }
    return count;
}

void get_help(void) {
    fprintf(stderr, "SYNOPSIS\n"
                    "   Compresses files using the LZ78 compression algorithm.\n"
                    "   Compressed files are decompressed with the corresponding decoder.\n"
                    "\n"
                    "USAGE\n"
                    "   ./test_encode [-vh] [-i input] [-o output]\n"
                    "\n"
                    "OPTIONS\n"
                    "   -v          Display compression statistics\n"
                    "   -i input    Specify input to compress (stdin by default)\n"
                    "   -o output   Specify output of compressed input (stdout by default)\n"
                    "   -h          Display program help and usage\n");
}

#define OPTIONS "vi:o:"
int main(int argc, char **argv) {
    bool verbose_checker = false;
    int infile = fileno(stdin); //default is set to stdin
    int outfile = fileno(stdout); //default is set to stdout
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
            outfile = open(optarg, O_WRONLY | O_CREAT | O_TRUNC);
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
    } //closing of the while loop for getopt

    //initialize the header
    FileHeader header;
    struct stat lab;
    fstat(infile, &lab);
    fchmod(outfile, lab.st_mode);
    header.magic = MAGIC; //setting the magic
    header.protection = lab.st_mode; //setting the protection

    write_header(outfile, &header); //write the header with the variables from above

    TrieNode *root = trie_create(); //initialize the TrieNode
    TrieNode *curr_node = root; //set the TrieNode's current node to root
    TrieNode *prev_node = NULL; //set the TrieNode's previous node to NULL
    uint8_t curr_sym = 0; //initialize current symbol to 0
    uint8_t prev_sym = 0; //initialize previos symbol to 0
    uint16_t next_code = START_CODE; //set next code to the value of START_CODE

    while (read_sym(infile, &curr_sym) == true) { //while it is still able to read a symbol
        TrieNode *next_node = trie_step(curr_node, curr_sym); //create a next node
        if (next_node != NULL) { //if next node is not NULL
            prev_node = curr_node; //set prev node to current node
            curr_node = next_node; //set current node to next node
        } else {
            write_pair(outfile, curr_node->code, curr_sym, BIT_LEN(next_code)); //write the pair
            curr_node->children[curr_sym] = trie_node_create(
                next_code); //set the chidren of current node at index current symbol to Trie create with next code
            curr_node = root; //set current node to root
            next_code = next_code + 1; //incrememt next code by 1
        }
        if (next_code == MAX_CODE) { //if next code reaches the value of MAX_CODE
            trie_reset(root); //reset the trie
            curr_node = root; //set current node to root
            next_code = START_CODE; //set next code to START_CODE
        }
        prev_sym = curr_sym; //set the previous symbol to current symbol
    }
    if (curr_node != root) { //if current node is not equal to root
        write_pair(outfile, prev_node->code, prev_sym, BIT_LEN(next_code)); //write the pair
        next_code = (next_code + 1) % MAX_CODE;
    }
    write_pair(outfile, STOP_CODE, prev_sym, BIT_LEN(next_code)); //write the pair
    flush_pairs(outfile); //flush any remaining pairs

    double ratio = 100 * (1 - ((double) total_bits / total_syms)); //calculate ratio
    if (verbose_checker == true) {
        printf("Compressed file size: %lu bytes\n", total_bits); //total bits calcuation
        printf("Uncompressed file size: %lu bytes\n", total_syms); //total symbols calculation
        printf("Compression ratio: %.2f%%\n", ratio); //ration calculation

        return 0;
    }

    return 0;

} //closing of main
