#include "mathlib.h"

#include <stdio.h>
//from lecture slides and miles section
static long double term = 1.0;
static int term_counter = 1;
double e(void) {
    long double sum = 1.0;
    for (long double k = 1; term > EPSILON; k += 1) {
        term *= 1 / k;
        sum += term;
        term_counter += 1;
    }
    return sum;
}

int e_terms(void) {
    return term_counter;
}
/*
int main() {
    printf("%lf\n", e());
    printf("%d\n", e_terms());
    return 0;
}
*/
