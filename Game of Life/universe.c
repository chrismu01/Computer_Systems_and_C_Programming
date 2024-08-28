#include "universe.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//struct for the universe
typedef struct Universe {
    uint32_t rows;
    uint32_t cols;
    bool **grid;
    bool toroidal;
} Universe;

//check out of bounds function: it checks if the input row and col is inside the range of the universe
bool check_out_bounds(Universe *u, uint32_t row, uint32_t col) {
    if ((row >= u->rows || row < 0) || (col >= u->cols || col < 0)) {
        return false;
    } else {
        return true;
    }
}

//uv create function: creates the universe and allocationg memory for it
Universe *uv_create(uint32_t rows, uint32_t cols, bool toroidal) {
    Universe *u = malloc(sizeof(Universe));
    u->rows = rows;
    u->cols = cols;
    u->toroidal = toroidal;
    bool **grid = (bool **) calloc(rows, sizeof(bool *));
    for (uint32_t r = 0; r < rows; r += 1) {
        grid[r] = (bool *) calloc(cols, sizeof(bool));
    }
    u->grid = grid;
    return u;
}

//uv delete function: frees memory used
void uv_delete(Universe *u) {
    for (uint32_t i = 0; i < u->rows; i += 1) {
        free(u->grid[i]);
    }
    free(u->grid); //free the grid
    free(u); //free the universe
}

//uv rows function: returns the number of rows in the universe
uint32_t uv_rows(Universe *u) {
    return (u->rows); //u points to rows
}

//uv cols function: to return the number of colmuns in the universe
uint32_t uv_cols(Universe *u) {
    return (u->cols); //u points to cols
}

//uv live cell function: marks the cell at r and c as alive (true)
void uv_live_cell(Universe *u, uint32_t r, uint32_t c) {
    if (check_out_bounds(u, r, c) == true) {
        u->grid[r][c] = true; //setting the cell as true
    }
}

//uv dead cell function: makrs the cell at r and c as dead (false)
void uv_dead_cell(Universe *u, uint32_t r, uint32_t c) {
    if (check_out_bounds(u, r, c) == true) {
        u->grid[r][c] = false; //setting the cell as false
    }
}

//uv get cell function: returns the value at r and c
bool uv_get_cell(Universe *u, uint32_t r, uint32_t c) {
    if (check_out_bounds(u, r, c) == true) {
        return u->grid[r][c]; //returns the values if its in the bounds
    } else {
        return false; //return false if r and c are out of bounds
    }
}

//uv populate function: populates the universe
bool uv_populate(Universe *u, FILE *infile) {
    uint32_t r = 0;
    uint32_t c = 0;
    while (
        fscanf(infile, "%u %u\n", &r, &c) != EOF) { //scnning the rows and columns until end of file
        if (check_out_bounds(u, r, c) == false) {
            return false;
        } else {
            uv_live_cell(u, r, c); //setting it as alive if its in bounds
        }
    }
    return true;
}
//from levs section
//uv census function: returns the number of live neighbors next to r and c
uint32_t uv_census(Universe *u, uint32_t r, uint32_t c) {
    uint32_t counter = 0; //counter variable
    bool check_toroidal = u->toroidal; //checks if it is toroidal
    uint32_t uni_row = u->rows; //initialize rows
    uint32_t uni_col = u->cols; //initialize cols
    for (int32_t row = -1; row <= 1; row += 1) { //looping from -1 to 1
        for (int32_t col = -1; col <= 1; col += 1) { //looping from -1 to 1
            if (row == 0 && col == 0) { //checking if the values of both row and col are 0
                continue;
            }

            uint32_t row_count = r + row;
            uint32_t col_count = c + col;

            if (check_toroidal) { // if it's toroidal
                row_count = (row_count + uni_row) % uni_row;
                col_count = (col_count + uni_col) % uni_col;
            } else if (row_count < 0 || row_count >= uni_row || col_count < 0
                       || col_count >= uni_col) {
                continue;
            }

            counter += uv_get_cell(u, row_count, col_count); //increment counter
        }
    }

    return counter;
}

//uv print function: prints out the universe
void uv_print(Universe *u, FILE *outfile) {
    for (uint32_t i = 0; i < u->rows; i += 1) {
        for (uint32_t j = 0; j < u->cols; j += 1) {
            if ((u->grid[i][j])) {
                fprintf(outfile, "o"); //this means it is dead so its prints a period
            } else {
                fprintf(outfile, "."); //this means it is alive so it prints an o
            }
        }
        fprintf(outfile, "\n");
    }
}

/*
int main(void) {
	printf("1\n");
	return 0;
}
*/
