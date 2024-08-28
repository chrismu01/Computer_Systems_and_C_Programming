#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <unistd.h>
#include <gmp.h>
#include "numtheory.h"
#include "randstate.h"
#include "ss.h"

extern gmp_randstate_t state;

void gcd(mpz_t g, const mpz_t a, const mpz_t b) {
    mpz_t t, temp_a, temp_b;
    mpz_inits(t, temp_a, temp_b, NULL);

    mpz_set(temp_a, a);
    mpz_set(temp_b, b);

    while (mpz_cmp_ui(temp_b, 0)) { //while loop with the condition b not equal to 0
        mpz_set(t, temp_b);
        mpz_mod(temp_b, temp_a, temp_b); //performs a%b and sets it to b
        mpz_set(temp_a, t);
    }
    mpz_set(g, temp_a); //this is variable that stores the GCD
    mpz_clears(t, temp_a, temp_b, NULL); //freeing all the space used by the variables
}

//mod inverse function
void mod_inverse(mpz_t o, const mpz_t a, const mpz_t n) { //returns inverse of a mod number

    //initialize variables
    mpz_t temp, r_temp, t_temp, q, r, t, rprime, tprime;
    mpz_init_set_ui(t, 0);
    mpz_init_set_ui(tprime, 1);
    mpz_init_set(r, n);
    mpz_init_set(rprime, a);
    mpz_init(q);
    //temp variables
    mpz_init(temp);
    mpz_init(t_temp);
    mpz_init(r_temp);

    while (mpz_cmp_ui(rprime, 0) != 0) { //while loop with the condition rprime not equal to 0
        mpz_fdiv_q(q, r, rprime); //q = (r/rprime)
        mpz_set(r_temp, r); //r_temp = r
        mpz_set(r, rprime); //r = rprime
        mpz_mul(temp, q, rprime);
        mpz_sub(rprime, r_temp, temp);
        mpz_set(t_temp, t);
        mpz_set(t, tprime); //t has the value of tprime now
        mpz_mul(temp, q, tprime); //q times tprime
        mpz_sub(tprime, t_temp, temp); //using the variable calculated above to calculate tprime
    }
    if (mpz_cmp_d(r, 1) > 0) { //if this if statment is satisfied, that means there is no inverser
        mpz_set_ui(o, 0);
        mpz_clears(r, t, q, tprime, rprime, temp, t_temp, r_temp, NULL);
        return;
    }

    if (mpz_cmp_d(t, 0) < 0) {
        mpz_add(t, t, n);
    }

    mpz_set(o, t); //setting o to t
    mpz_clears(r, t, q, tprime, rprime, temp, t_temp, r_temp,
        NULL); //freeing all the space used by the variables
}

//pow mod function
void pow_mod(mpz_t o, const mpz_t a, const mpz_t d,
    const mpz_t n) { //calculates the base raised to the exponenet power modolu modulus
    mpz_t v, p, temp_d; //intializing varables to be used

    mpz_init_set_ui(v, 1); //setting v to 1

    mpz_init_set(p, a); //setting the value of a to p

    mpz_init(temp_d);
    mpz_set(temp_d, d);

    while (mpz_cmp_ui(temp_d, 0) > 0) {
        if (mpz_odd_p(temp_d) == 1) {
            mpz_mul(v, v, p); //multiplying v by p and setting it to v
            mpz_mod(v, v, n); //performing v mod n and setting it to v
        }
        mpz_mul(p, p, p); //multplying p with p and setting it to p
        mpz_mod(p, p, n); // p mod n setting it to p
        mpz_fdiv_q_ui(temp_d, temp_d, 2); //divide d by 2 setting it to d
    }
    mpz_set(o, v); //powmod result
    mpz_clear(p); //clearing used variable
    mpz_clear(v);
}
//got help from lev the tutor
bool is_prime(const mpz_t n, uint64_t iters) {
    //made these cases for prime numbers it has problems finding
    if ((mpz_cmp_d(n, 0) == 0)
        || (mpz_cmp_d(n, 1) == 0)) { //my is prime has trouble returning false for 0 and 1
        return false;
    }

    if ((mpz_cmp_d(n, 2) == 0) || (mpz_cmp_d(n, 3) == 0)
        || (mpz_cmp_d(n, 5) == 0)) { //my is prime has trouble returning true for 2 and 3
        return true;
    }

    //initializing variables
    mpz_t var1, var2, var3, temp1, temp2, r, y, j, h, c, f, i;
    mpz_inits(var1, var2, var3, temp1, temp2, r, y, j, h, c, f, i, NULL);

    mpz_set_ui(var1, 1); //setting var1 equal to 1
    mpz_set_ui(var2, 1); //setting var2 equal to 1
    mpz_mul_ui(var1, var1, 2);
    mpz_sub_ui(temp1, n, 1);
    mpz_fdiv_q(temp2, temp1, var1);
    mpz_set(r, temp2);

    unsigned long int even_checker;
    while ((even_checker = mpz_tdiv_ui(r, 2)) == 0) {
        mpz_add_ui(var2, var2, 1); //var2 plus 1 setting to var2
        mpz_mul_ui(var1, var1, 2); //var1 times 2 setting to var1
        mpz_sub_ui(temp1, n, 1); //n time 1 setting it to temp1
        mpz_fdiv_q(temp2, temp1, var1);
        mpz_set(r, temp2);
        even_checker = mpz_tdiv_ui(r, 2);
    }
    for (; mpz_cmp_ui(i, iters) < 0; mpz_add_ui(i, i, 1)) {
        mpz_sub_ui(c, n, 3);
        mpz_urandomm(var3, state, c); //fetching a random number and setting that to t
        mpz_add_ui(var3, var3, 2);
        pow_mod(y, var3, r, n);
        if ((mpz_cmp_ui(y, 1) != 0)) { //checks if y is not equal to 1
            if (mpz_cmp(y, temp1) != 0) { //checks if y is not equal to y not equal to  n-1
                mpz_set_ui(j, 1);
                mpz_sub_ui(h, var2, 1);
                while ((mpz_cmp(j, h) <= 0) && (mpz_cmp(y, temp1) != 0)) {
                    mpz_set_ui(var3, 2);
                    pow_mod(y, y, var3, n);

                    if (mpz_cmp_ui(y, 1) == 0) {
                        //clear variables
                        mpz_clears(var1, var2, var3, temp1, temp2, r, y, j, h, c, f, i, NULL);
                        return false; //not prime
                    }
                    mpz_add_ui(f, j, 1);
                    mpz_set(j, f);
                }
                if (mpz_cmp(y, temp1) != 0) {
                    return false; //not prime
                }
            }
        }
    }

    //clear variables
    mpz_clears(var1, var2, var3, temp1, temp2, r, y, j, h, c, f, i, NULL);
    return true; //is prime
}

//make prime function
void make_prime(mpz_t p, uint64_t bits, uint64_t iters) { //creates prime number

    mpz_t num, offset;
    mpz_inits(num, offset, NULL);

    uint64_t temp_bits = bits; //temping the bits to use later (to not accidentally overwrite it)
    uint64_t temp_iters = iters; //temping the iters to use later (to not accidentally overwrite it)

    mpz_set_ui(offset, 2);
    mpz_pow_ui(offset, offset, bits); //offset is 2^n-1

    mpz_urandomb(num, state, temp_bits); //fetch a random number
        //
    //checking if its a prime number and it keeps checking unitl found
    while (!is_prime(num, temp_iters)) {
        mpz_urandomb(num, state, temp_bits); //fetch a random number
        mpz_add(num, num, offset); //add the offset
    }

    mpz_set(p, num); //prime number

    //clear variables
    mpz_clears(num, offset, NULL);
}
