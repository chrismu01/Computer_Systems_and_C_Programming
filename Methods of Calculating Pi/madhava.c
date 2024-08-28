#include "mathlib.h"

#include <stdio.h>

static int terms = 1;

double pi_madhava(void) {
    //numerator = (-3)^-k
    //denomominator = 2k+1
    double mad_result = 0.0;
    double n = 1.0; //numerator
    double d = 1.0; //denominator
    for (int k = 1; (absolute(n / d) > EPSILON); k += 1) {
        mad_result = mad_result + (n / d);
        n *= (-1.0 / 3.0);
        d = ((2.0 * k) + 1.0);
        terms += 1;
    }

    mad_result *= sqrt_newton(12);
    return mad_result;
}

int pi_madhava_terms(void) {
    return terms;
}
/*
int main() {
    printf("%lf\n", pi_madhava());
    printf("%d\n", pi_madhava_terms());
    return 0;
}
*/
