#include "batcher.h"

#include "set.h"
#include "stats.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

//comparator function
void comparator(
    Stats *stats, uint32_t A[], uint32_t x, uint32_t y) { //def comparator(A: list, x: int, y: int):
    if (cmp(stats, A[x], A[y]) == 1) { //if A[x] > A[y]: using compare function
        swap(stats, &A[x], &A[y]); //swap A[x] and A[y]. A[x], A[y] = A[y], A[x]
    }
}

//bit length function
uint32_t bit_length(uint32_t n) {
    uint32_t t = 0;
    while (n > 0) {
        n >>= 1; //right shift
        t += 1;
    }
    return t;
}

//batcher sort function
void batcher_sort(Stats *stats, uint32_t *A, uint32_t n) { //def batcher_sort(A: list):
    if (n == 0) { //if the length is zero
        return;
    }
    uint32_t t = 0;
    t = bit_length(n); //calling the bit length function

    uint32_t p = 1 << (t - 1); //performing a left shift

    while (p > 0) { //while p > 0:
        uint32_t q, r, d = 0; //intializing variables
        q = (1 << (t - 1));
        r = 0;
        d = p;
        while (d > 0) { //while d > 0:
            for (uint32_t i = 0; i < (n - d); i += 1) { //for i in range (0,n - d)
                if ((i & p) == r) { //if (i & p) == r:
                    comparator(stats, A, i, (i + d)); //call the comparator function
                }
            }
            d = (q - p);
            q >>= 1; //right shift
            r = p; //setting r to p
        }
        p >>= 1; //right shift
    }
}
