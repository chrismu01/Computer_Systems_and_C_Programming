#include "mathlib.h"

#include <stdio.h>

//help from eric the tutor

static int counter = 0;
double pi_viete(void) {
    //ak = sqrt(2 + a k-1)
    long double n = sqrt_newton(2.0); //numerator
    long double d = 2.0; //denonminator
    long double guess = n / d;
    long double previous = 0.0;
    double to_return = 0.0;
    while (absolute(guess - previous) > EPSILON) {
        n = sqrt_newton(2.0 + n);
        previous = guess;
        guess *= (n / d);
        counter += 1;
    }
    to_return = 2 / guess;
    return to_return;
}

int pi_viete_factors(void) {
    return counter;
}
/*
int main() {
    printf("%lf\n", pi_viete());
    printf("%d\n", pi_viete_factors());
    return 0;
}
*/
