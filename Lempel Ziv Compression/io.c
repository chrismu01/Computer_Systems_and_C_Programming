#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>
#include "io.h"
#include "endian.h"
#include "code.h"
#include <unistd.h>

uint64_t total_bits, total_syms;
//help from john's section
//read bytes function: reads the bytes from the infile
int read_bytes(int infile, uint8_t *buf, int to_read) {
    int bytes_read = 0;
    while (bytes_read < to_read) { //loop until three are no more bytes to read
        int read_bytes = read(infile, buf + bytes_read,
            to_read
                - bytes_read); //reading the infile using parameters infile, (buf + bytes_read), and (to_read - bytes_read)
        if (read_bytes <= 0) { //break if result is 0
            break;
        }
        bytes_read += read_bytes;
    }
    total_syms += bytes_read;
    return bytes_read; //return bytes_read
}

//help from johns section
//write bytes function: write the bytes into outfile
int write_bytes(int outfile, uint8_t *buf, int to_write) {
    if (to_write == 0) { //this means there is nothing to write
        return 0; //return 0
    }
    int bytes_write = 0;
    while (bytes_write < to_write) { //loop until there are no more bytes to write
        int bytes_wrote = write(outfile, buf + bytes_write,
            to_write
                - bytes_write); //writing to outfile using parameters outfile, (buf + bytes_write), and (to_write - bytes_write)
        if (bytes_wrote == 0) { //if the result is 0 break
            break;
        }
        bytes_write += bytes_wrote; //increment bytes_write
    }
    total_bits += bytes_write;
    return bytes_write; //return bytes_write
}
/*
void read_header(int infile, FileHeader *header) {
}
*/
//write header function: write sizeof(FileHeader)
void write_header(int outfile, FileHeader *header) {
    if (!little_endian()) { // if little endian is false
        header->magic = swap32(header->magic); //set the magic
        header->protection = swap16(header->protection); //set the protection
    }

    // Write header to output file
    if (write_bytes(outfile, (uint8_t *) header, sizeof(FileHeader)) != sizeof(FileHeader)) {
        perror("Error");
        exit(EXIT_FAILURE);
    }
}

static size_t buf_ender = 0; //initialize when the buffer should end
static size_t buf_position = 0; //initialize the position of the buffer
static uint8_t buf[BLOCK]; //set up the buffer with the size of it with BLOCK
//for encode
//read symbol function: used to process symbols
bool read_sym(int infile, uint8_t *sym) { //reads in the infile and symbol
    if (buf_position == buf_ender) {
        buf_ender = read_bytes(infile, buf, BLOCK); //read the bytes
        buf_position = 0; //reset the buffer position to 0
        if (buf_ender == 0) {
            return false;
        }
    }
    *sym = buf[buf_position]; //set symbol pointer to the buffer at index buffer position
    buf_position++; //increment buffer position
    return true;
}

static int buff_pair_pos = 0; //position of the buffer is intialized at 0
static uint8_t buff_pair[BLOCK]; //buffer initialized with size of block to be used later

//help from lev
//for encode
//write pair function: write a pair to the outfile. A pair contains a code and a symbol
void write_pair(int outfile, uint16_t code, uint8_t sym, int bitlen) {
    for (int iters = 0; iters < bitlen; iters++) { //loop through the bit length
        uint8_t bit = (code >> iters) & 0x1;
        buff_pair[buff_pair_pos / 8] |= (bit << (buff_pair_pos % 8));
        buff_pair_pos++;

        if (buff_pair_pos >= BLOCK * 8) {
            if (write_bytes(outfile, buff_pair, BLOCK) != BLOCK) {
                perror("Error");
                exit(EXIT_FAILURE);
            }
            buff_pair_pos = 0;
        }
    }
    for (int iters = 0; iters < 8; iters++) { //loop through unitl 8 is reached
        uint8_t bit = (sym >> iters) & 0x1;
        buff_pair[buff_pair_pos / 8] |= (bit << (buff_pair_pos % 8));
        buff_pair_pos++; //increment the buffer position by 1
        if (buff_pair_pos >= BLOCK * 8) {
            if (write_bytes(outfile, buff_pair, BLOCK)
                != BLOCK) { //if write bytes is not equal to the value of BLOCK
                perror("Error");
                exit(EXIT_FAILURE);
            }
            buff_pair_pos = 0; //reset position of the buffer to 0
        }
    }
}
//help from lev
//for encode
//flush pairs function: write out remaining pairs to the output file
void flush_pairs(int outfile) {
    int bytes_to_write
        = (buff_pair_pos / 8)
          + (buff_pair_pos % 8 == 0
                  ? 0
                  : 1); //ceiling division. if it is divisable by 8 then its sets to 0 else 1
    write_bytes(outfile, buff_pair, bytes_to_write); //write the bytes
    buff_pair_pos = 0; // reset count
}

/*
//static int bit_count = 0;
//for decode
bool read_pair(int infile, uint16_t *code, uint8_t *sym, int bitlen) {
}

//for decode
void write_word(int outfile, Word *w) {

}
//for decode
void flush_words(int outfile) {

}
*/
