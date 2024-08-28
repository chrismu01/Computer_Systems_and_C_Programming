#include "quick.h"

#include "stats.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

//partition function
int partition(Stats *stats, uint32_t A[], uint32_t lo, uint32_t hi) {
    uint32_t i = 0; //initializing i
    i = (lo - 1); //i - lo - 1;
    for (uint32_t j = lo; j < hi; j += 1) {
        if (cmp(stats, A[j - 1], A[hi - 1])
            == -1) { //using compare function to see if A[j - 1] is less than A[hi - 1]
            i += 1; //increase i by 1
            swap(stats, &A[j - 1], &A[i - 1]); //swap the two using swap function
        }
    }
    swap(stats, &A[i], &A[hi - 1]); //swap the two
    return (i + 1);
}

//quick sorter function
void quick_sorter(Stats *stats, uint32_t A[], uint32_t lo, uint32_t hi) {
    int p = 0;
    if (lo < hi) { //if lo < hi:
        p = partition(stats, A, lo, hi);
        quick_sorter(stats, A, lo, p - 1);
        quick_sorter(stats, A, p + 1, hi);
    }
}

//quick sort function
void quick_sort(Stats *stats, uint32_t *A, uint32_t n) {
    quick_sorter(stats, A, 1, n);
}
