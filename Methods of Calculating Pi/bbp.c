#include "mathlib.h"

#include <stdio.h>

static int term_counter = 0;
double pi_bbp(void) {
    //numerator = (k(120k + 151) + 47)
    //denominator = k(k(k(512k + 1024) + 712) + 194) +15
    long double d = 1.0; //denominator
    long double f1 = 0.0; // 4 / (8k+1)
    long double f2 = 0.0; // 2 / (8k+4)
    long double f3 = 0.0; // 1 / (8k+5)
    long double f4 = 0.0; // 1 / (8k+6)
    long double term = 1.0;
    long double sum = 0.0;
    for (long double k = 0.0; absolute(term) > EPSILON; k += 1) {
        f1 = (4.0 / (8.0 * k + 1.0));
        f2 = (2.0 / (8.0 * k + 4.0));
        f3 = (1.0 / (8.0 * k + 5.0));
        f4 = (1.0 / (8.0 * k + 6.0));
        term = (d * (f1 - f2 - f3 - f4));
        d /= 16.0;
        sum += term;
        term_counter += 1;
    }
    return sum;
}

int pi_bbp_terms(void) {
    return term_counter;
}

/*
int main() {
	printf("%f\n", pi_bbp());
	printf("%d\n", pi_bbp_terms());
    return 0;
}
*/
