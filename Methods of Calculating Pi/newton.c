#include "mathlib.h"

#include <stdio.h>

static int iters = 0;
double sqrt_newton(double x) {
    long double z = 0.0;
    long double y = 1.0;
    iters = 0.0;
    while (absolute(y - z) > EPSILON) {
        z = y;
        y = 0.5 * (z + x / z);
        iters += 1;
    }
    return y;
}

int sqrt_newton_iters(void) {
    return iters;
}
/*
int main() {
	return 0;
}
*/
