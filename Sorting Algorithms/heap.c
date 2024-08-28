#include "heap.h"

#include "gaps.h"
#include "stats.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

//max child function
uint32_t max_child(Stats *stats, uint32_t A[], uint32_t first, uint32_t last) {
    uint32_t left, right = 0; //intializing left and right variables
    left = 0;
    right = 0;
    left = 2 * first; //left = 2 * first
    right = left + 1; //right = left + 1
    if ((right <= last) && (cmp(stats, A[right - 1], A[left - 1]) == 1)) {
        return right;
    }
    return left;
}

//fix heap function
void fix_heap(Stats *stats, uint32_t A[], uint32_t first, uint32_t last) {
    bool found = false; //intializing found to false
    uint32_t mother = first;
    uint32_t great = max_child(stats, A, mother, last);

    while (mother <= last / 2 && !found) { // 2 && not found
        if (cmp(stats, A[mother - 1], A[great - 1]) == -1) {
            swap(stats, &A[mother - 1], &A[great - 1]); //swap the two
            mother = great;
            great = max_child(stats, A, mother, last);
        } else {
            found = true; //set found to true
        }
    }
}

//build heap function
void build_heap(Stats *stats, uint32_t A[], uint32_t first, uint32_t last) {
    for (uint32_t father = last / 2; father > first - 1;
         father -= 1) { //for father in range(last // 2, first -1. -1)
        fix_heap(stats, A, father, last); //calling fix heap function
    }
}

//heap sort function
void heap_sort(Stats *stats, uint32_t *A, uint32_t n) {
    uint32_t first, last = 0; //intializing variables
    first = 1;
    last = n;
    build_heap(stats, A, first, last); //calling the build heap function
    for (uint32_t leaf = last; leaf > first; leaf -= 1) {
        swap(stats, &A[first - 1], &A[leaf - 1]); //calling the swap function
        fix_heap(stats, A, first, (leaf - 1));
    }
}
/*
int main() {
	return 0;

}
*/
