#include "mathlib.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static long double mathlib = 0.0;
void gethelp() {
    fprintf(stderr, "SYNOPSIS"
                    "   A test harness for the small numerical library.\n"
                    "\n"
                    "USAGE\n"
                    "   ./mathlib-test [-aebmrvnsh]\n"
                    "\n"
                    "OPTIONS\n"
                    "  -a   Runs all tests.\n"
                    "  -e   Runs e test.\n"
                    "  -b   Runs BBP pi test.\n"
                    "  -m   Runs Madhava pi test.\n"
                    "  -r   Runs Euler pi test.\n"
                    "  -v   Runs Viete pi test.\n"
                    "  -n   Runs Newton square root tests.\n"
                    "  -s   Print verbose statistics.\n"
                    "  -h   Display program synopsis and usage.\n");
}

#define OPTIONS "aebmrvnsh"
int main(int argc, char **argv) {
    int opt = 0;
    bool a = false;
    bool e_checker = false;
    bool b = false;
    bool m = false;
    bool r = false;
    bool v = false;
    bool n = false;
    bool s = false;
    bool h = false;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'a': //Runs all tests
            a = true;
            break;
        case 'e': //Runs e approximation test
            e_checker = true;
            break;
        case 'b': //
            b = true;
            break;
        case 'm': m = true; break;
        case 'r': r = true; break;
        case 'v': v = true; break;
        case 'n': n = true; break;
        case 's': s = true; break;
        case 'h': h = true; break;
        default: h = true; break;
        }
    }

    if (s == true && a == false && e_checker && b == false && m == false && r == false && v == false
        && n == false) {
        gethelp();
    }

    if (h == true) {
        gethelp();
    }
    if (a == true) {
        e_checker = true;
        b = true;
        m = true;
        r = true;
        v = true;
        n = true;
    }
    //activates if case e is true
    if (e_checker == true) {
        long double output = 0.0;
        long double difference = 0.0;
        int terms = 0;
        output = e();
        terms = e_terms();
        mathlib = M_E;
        difference = mathlib - output;
        if (s == true) {
            printf("e() = %.15Lf, M_E = %.15Lf, diff = %.15Lf\n", output, mathlib, difference);
            printf("e() terms = %2d\n", terms);
        } else {
            printf("e() = %.15Lf, M_E = %.15Lf, diff = %.15Lf\n", output, mathlib, difference);
        }
    }

    //activates if case b is true
    if (b == true) {
        long double output = 0.0;
        long double difference = 0.0;
        int terms = 0;
        output = pi_bbp();
        terms = pi_bbp_terms();
        mathlib = M_PI;
        difference = mathlib - output;
        if (s == true) {
            printf(
                "pi_bbp() = %.15Lf, M_PI = %.15Lf, diff = %.15Lf\n", output, mathlib, difference);
            printf("pi_bbp_terms() terms = %2d\n", terms);
        } else {
            printf(
                "pi_bbp() = %.15Lf, M_PI = %.15Lf, diff = %.15Lf\n", output, mathlib, difference);
        }
    }

    //activates if case m is true
    if (m == true) {
        long double output = 0.0;
        long double difference = 0.0;
        int terms = 0;
        output = pi_madhava();
        terms = pi_madhava_terms();
        mathlib = M_PI;
        difference = mathlib - output;
        if (s == true) {
            printf("pi_madhava() = %.15Lf, M_PI = %.15Lf, diff = %.15Lf\n", output, mathlib,
                difference);
            printf("pi_madhava_terms() terms = %2d\n", terms);
        } else {
            printf("pi_madhava() = %.15Lf, M_PI = %.15Lf, diff = %.15Lf\n", output, mathlib,
                difference);
        }
    }

    //case r
    if (r == true) {
        long double output = 0.0;
        long double difference = 0.0;
        int terms = 0;
        output = pi_euler();
        terms = pi_euler_terms();
        mathlib = M_PI;
        difference = mathlib - output;
        if (s == true) {
            printf(
                "pi_euler() = %.15Lf, M_PI = %.15Lf, diff = %.15Lf\n", output, mathlib, difference);
            printf("pi_euler_terms() terms = %2d\n", terms);
        } else {
            printf(
                "pi_euler() = %.15Lf, M_PI = %.15Lf, diff = %.15Lf\n", output, mathlib, difference);
        }
    }

    //case v
    if (v == true) {
        long double output = 0.0;
        long double difference = 0.0;
        int terms = 0;
        output = pi_viete();
        terms = pi_viete_factors();
        mathlib = M_PI;
        difference = mathlib - output;
        if (s == true) {
            printf(
                "pi_viete() = %.15Lf, M_PI = %.15Lf, diff = %.15Lf\n", output, mathlib, difference);
            printf("pi_viete_factors() terms = %2d\n", terms);
        } else {
            printf(
                "pi_viete() = %.15Lf, M_PI = %.15Lf, diff = %.15Lf\n", output, mathlib, difference);
        }
    }

    //case n
    if (n == true) {
        if (s == true) {
            for (double i = 0.0; i <= 10.0; i += 0.1) {
                long double output = 0.0;
                long double difference = 0.0;
                int terms = 0;
                long double mathlib_sqrt = 0.0;
                output = sqrt_newton(i);
                terms = sqrt_newton_iters();
                mathlib_sqrt = sqrt(i);
                difference = absolute(mathlib_sqrt - output);
                printf("sqrt_newton(%.6f) = %.15Lf, sqrt(%.6f) = %.15Lf, diff = %.15Lf\n", i,
                    output, i, mathlib_sqrt, difference);
                printf("sqrt_newton() terms = %2d\n", terms);
            }
        } else {
            for (double i = 0.0; i <= 10.0; i += 0.1) {
                long double output = 0.0;
                long double difference = 0.0;
                long double mathlib_sqrt = 0.0;
                output = sqrt_newton(i);
                mathlib_sqrt = sqrt(i);
                difference = absolute(mathlib_sqrt - output);
                printf("sqrt_newton(%.6f) = %.15Lf, sqrt(%.6f) = %.15Lf, diff = %.15Lf\n", i,
                    output, i, mathlib_sqrt, difference);
            }
        }
    }

    return 0;
}
