#include "universe.h"

#include <inttypes.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//-h function
void get_help(void) {
    fprintf(stderr, "SYNOPSIS"
                    "    Conway's Game of Life\n"
                    "\n"
                    "USAGE\n"
                    "    ./life_amd64 tsn:i:o:h\n"
                    "\n"
                    "OPTIONS\n"
                    "    -t             Create your universe as a toroidal\n"
                    "    -s             Silent - do not use animate the evolution\n"
                    "    -n {number}    Number of generations [default: 100]\n"
                    "    -i {file}      Input file [default: stdin]\n"
                    "    -o {file}      Output file [default: stdout]\n");
}

#define OPTIONS "tsn:i:o:h"
int main(int argc, char **argv) {
    int generations = 100;
    //bool check_file = false;
    FILE *infile;
    FILE *outfile;
    char *infile_name = NULL;
    char *outfile_name = NULL;
    int opt = 0;
    bool toroidal = false;
    bool silence_ncurses = false;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 't': //start the game
            toroidal = true;
            break;
        case 's': //silence ncurses
            silence_ncurses = true;
            break;
        case 'n': //number of generations
            generations = atoi(optarg);
            break;
        case 'i': //input file to read
            infile_name = optarg;
            break;
        case 'o': //output file to write in
            outfile_name = optarg;
            //check_file = true;
            break;
        case 'h': //help case
            get_help();
            return 0;
            break;
        default:
            get_help();
            return 0;
            break;
        }
    }

    uint32_t rows, cols = 0;
    infile = infile_name != NULL ? fopen(infile_name, "r") : stdin; //opens infile
    outfile = outfile_name != NULL ? fopen(outfile_name, "w") : stdout; //opens outfile
    fscanf(infile, "%u %u", &rows, &cols);
    Universe *A = uv_create(rows, cols, toroidal); //creating universe A
    Universe *B = uv_create(rows, cols, toroidal); //creating universe B
    uv_populate(A, infile); //populating universe A with infie

    if (silence_ncurses == false) {
        initscr(); //initialize screen
        curs_set(FALSE); //hide the cursor
    }

    for (int i = 0; i < generations; i += 1) { //looping through generations
        clear();
        uint32_t num_rows = uv_rows(A); //rows in universe A
        uint32_t num_cols = uv_cols(A); //cols in universe A
        for (uint32_t rows = 0; rows < num_rows; rows += 1) {
            for (uint32_t cols = 0; cols < num_cols; cols += 1) {
                if (uv_get_cell(A, rows, cols) == true) {
                    mvprintw(rows, cols, "o");
                }

                //rule number 1 of the game
                if ((uv_get_cell(A, rows, cols) == true)
                    && ((uv_census(A, rows, cols) == 2) || (uv_census(A, rows, cols) == 3))) {
                    uv_live_cell(B, rows, cols);
                }
                //rule 2 of the game
                else if ((uv_get_cell(A, rows, cols) == false) && (uv_census(A, rows, cols) == 3)) {
                    uv_live_cell(B, rows, cols);
                } else {
                    uv_dead_cell(B, rows, cols);
                }
            }
        }
        if (silence_ncurses == false) {
            refresh(); //refresh window
            usleep(50000); //sleep for 50000 microseconds
        }
        //swapping the universes
        Universe *C = A;
        A = B;
        B = C;
    }
    if (silence_ncurses == false) {
        endwin(); //close screen
    }

    uv_print(A, outfile);

    uv_delete(A); //delete universe A
    uv_delete(B); //delete universe B
    fclose(infile); //closes the in file
    fclose(outfile); //closes the outfile

    return 0;
}
