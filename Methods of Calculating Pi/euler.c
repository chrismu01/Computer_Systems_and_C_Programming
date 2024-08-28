#include "mathlib.h"

#include <stdio.h>

static long double term = 1.0;
static int term_counter = 0;
double pi_euler(void) {
    long double euler_result;
    long double sum = 0.0;
    for (long double k = 1; term > EPSILON; k += 1) {
        long double denom = k * k;
        term = 1 / denom;
        sum += term;
        term_counter += 1;
    }
    euler_result = sqrt_newton((6 * sum));
    return euler_result;
}

int pi_euler_terms(void) {
    return term_counter;
}
