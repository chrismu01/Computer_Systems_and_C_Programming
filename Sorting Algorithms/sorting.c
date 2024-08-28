#include "batcher.h"
#include "gaps.h"
#include "heap.h"
#include "quick.h"
#include "set.h"
#include "shell.h"
#include "stats.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
void gethelp(void) {
    fprintf(stderr, "Select at least one sort to perform.\n"
                    "SYNOPSIS\n"
                    "   A collection of comparison-based sorting algorithms.\n"
                    "\n"
                    "USAGE\n"
                    "   ./sorting [-Hasbhqn:p:r:] [-n length] [-p elements] [-r seed]\n"
                    "\n"
                    "OPTIONS\n"
                    "  -H              Display program help and usage.\n"
                    "  -a              Enable all sorts.\n"
                    "  -s              Enable Shell Sort.\n"
                    "  -b              Enable Batcher Sort.\n"
                    "  -h              Enable Heap Sort.\n"
                    "  -q              Enable Quick Sort.\n"
                    "  -n length       Specify number of array elements (default: 100).\n"
                    "  -p elements     Specify number of elements to print (default: 100).\n"
                    "  -r seed         Specify random seed (default: 13371453).\n");
}

#define OPTIONS "ahbsqr:n:p:H"
int main(int argc, char **argv) {
    uint64_t arr_seed = 13371453;
    uint64_t arr_size = 100;
    uint64_t print_element = 100;
    Stats stats;
    reset(&stats);
    Set my_set = set_empty();
    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'a':
            my_set = set_insert(my_set, 1);
            my_set = set_insert(my_set, 2);
            my_set = set_insert(my_set, 3);
            my_set = set_insert(my_set, 4);
            break;
        case 'h': //Heap Sort
            my_set = set_insert(my_set, 1);
            break;
        case 'b': //Batcher Sort
            my_set = set_insert(my_set, 2);
            break;
        case 's': //Shell Sort
            my_set = set_insert(my_set, 3);
            break;
        case 'q': //Quicksort
            my_set = set_insert(my_set, 4);
            break;
        case 'r': //Set custom seed
            arr_seed = atoi(optarg);
            break;
        case 'n': //Set array custom size
            arr_size = atoi(optarg);
            break;
        case 'p': //Prints out elements
            print_element = atoi(optarg);
            break;
        case 'H': //Program usage
            gethelp();
            break;
        default: gethelp(); break;
        }
    }
    srandom(arr_seed);
    uint32_t *arr = (uint32_t *) calloc(arr_size, sizeof(uint32_t));
    for (uint32_t i = 0; i < arr_size; i++) {
        arr[i] = random() & 0x3FFFFFFF;
    }

    //heap sort
    if (set_member(my_set, 1)) {
        heap_sort(&stats, arr, arr_size);
        printf("Heap Sort, %lu elements, %lu moves, %lu compares\n", arr_size, stats.moves,
            stats.compares);
        uint32_t stopper;
        if (arr_size < print_element) {
            stopper = arr_size;
        } else {
            stopper = print_element;
        }
        for (uint32_t iters = 0; iters < stopper; iters++) {
            printf("%13" PRIu32 "", arr[iters]);
            if ((iters + 1) % 5 == 0) {
                printf("\n");
            }
        }
        reset(&stats);
    }

    //batcher sort
    if (set_member(my_set, 2)) {
        batcher_sort(&stats, arr, arr_size);
        printf("Batcher Sort, %lu elements, %lu moves, %lu compares\n", arr_size, stats.moves,
            stats.compares);
        uint32_t stopper;
        if (arr_size < print_element) {
            stopper = arr_size;
        } else {
            stopper = print_element;
        }
        for (uint32_t iters = 0; iters < stopper; iters++) {
            printf("%13" PRIu32 "", arr[iters]);
            if ((iters + 1) % 5 == 0) {
                printf("\n");
            }
        }
        reset(&stats);
    }

    //shell sort
    if (set_member(my_set, 3)) {
        shell_sort(&stats, arr, arr_size);
        printf("Shell Sort, %lu elements, %lu moves, %lu compares\n", arr_size, stats.moves,
            stats.compares);
        uint32_t stopper;
        if (arr_size < print_element) {
            stopper = arr_size;
        } else {
            stopper = print_element;
        }
        for (uint32_t iters = 0; iters < stopper; iters++) {
            printf("%13" PRIu32 "", arr[iters]);
            if ((iters + 1) % 5 == 0) {
                printf("\n");
            }
        }
        reset(&stats);
    }

    //quick sort
    if (set_member(my_set, 4)) {
        quick_sort(&stats, arr, arr_size);
        printf("Quick Sort, %lu elements, %lu moves, %lu compares\n", arr_size, stats.moves,
            stats.compares);
        uint32_t stopper;
        if (arr_size < print_element) {
            stopper = arr_size;
        } else {
            stopper = print_element;
        }
        for (uint32_t iters = 0; iters < stopper; iters++) {
            printf("%13" PRIu32 "", arr[iters]);
            if ((iters + 1) % 5 == 0) {
                printf("\n");
            }
        }
        reset(&stats);
    }

    free(arr);

    return 0;
}
