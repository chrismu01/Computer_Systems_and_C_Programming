#include "shell.h"

#include "gaps.h"
#include "stats.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void shell_sort(Stats *stats, uint32_t *arr, uint32_t length) {
    for (uint32_t g = 0; g < GAPS; g += 1) { //for (gap in gaps){
        uint32_t gap = gaps[g]; //calling gaps array from gaps.h
        for (uint32_t i = gap; i < length; i += 1) {
            uint32_t j = i;
            uint32_t temp = arr[j]; //temp = arr[i];
            move(stats, arr[j]); //data for the stats
            while ((j >= gap) && (cmp(stats, temp, arr[j - gap]) == -1)) {
                arr[j] = arr[j - gap];
                move(stats, arr[j - gap]); //data for the stats
                j -= gap;
            }
            arr[j] = temp;
            move(stats, arr[j]); //data for the stats
        }
    }
}
