#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>
#include "time.h"
#include "ss.h"
#include "randstate.h"
#include <sys/stat.h>

void get_help(void) {
    fprintf(stderr, "SYNOPSIS\n"
                    "   Generates an SS public/private key pair.\n"
                    "\n"
                    "USAGE\n"
                    "   ./keygen [OPTIONS]\n"
                    "\n"
                    "OPTIONS\n"
                    "   -h              Display program help and usage.\n"
                    "   -v              Display verbose program output.\n"
                    "   -b bits         Minimum bits needed for public key n (default: 256).\n"
                    "   -i iterations   Miller-Rabin iterations for testing primes (default: 50).\n"
                    "   -n pbfile       Public key file (default: ss.pub).\n"
                    "   -d pvfile       Private key file (default: ss.priv).\n"
                    "   -s seed         Random seed for testing.\n");
}

#define OPTIONS "b:i:n:d:s:vh"
int main(int argc, char **argv) {
    int minimum_bits = 256; //default number of bits
    int miller_iters = 50; //default iterations of the miller
    FILE *pbfile;
    FILE *pvfile;
    char *pbfile_name = "ss.pub";
    char *pvfile_name = "ss.priv";
    uint32_t seed = time(NULL); //fetches the seconds of the times and use that number as seed
    bool check_verbose = false;
    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'b': //number of bits for the public nodulus n
            minimum_bits = atoi(optarg);
            break;

        case 'i': //number of miller rabin iterations for testing primes
            miller_iters = atoi(optarg);
            break;

        case 'n': //public key file
            pbfile_name = optarg;
            break;

        case 'd': //private key file
            pvfile_name = optarg;
            break;

        case 's': //specify the seed
            seed = atoi(optarg);
            break;

        case 'v': //print the  verbose
            check_verbose = true;
            break;

        case 'h': //help case
            get_help();
            return 0;

        } //end of switch cases
    } //close of the while loop

    pbfile = fopen(pbfile_name, "w"); //open the public key file to write
    pvfile = fopen(pvfile_name, "w"); //open the private key file to write

    //Step 3: make the private key file permissions set to 0600
    fchmod(fileno(pvfile), 0600);

    //Step 4: initialize the randstate using the set sead
    randstate_init(seed);
    mpz_t n, p, q, a, pq, d;
    mpz_inits(n, p, q, a, pq, d, NULL); //initializing variables

    //Step 5: make the public and private keys
    ss_make_pub(p, q, n, minimum_bits, miller_iters);
    ss_make_priv(d, pq, p, q);

    //Step 6: get the user's name
    char *username = getenv("USER");

    //Step 7: write the keys in the respective files
    ss_write_pub(n, username, pbfile);
    ss_write_priv(pq, d, pvfile);

    //Step 8: checking if verbose is enables and printing the following if it is
    if (check_verbose == true) {
        gmp_printf("user = %s\n", username); //prints username

        size_t p_bits;
        p_bits = mpz_sizeinbase(p, 2);
        gmp_printf("p (%lu bits) = %Zd\n", p_bits, p); //prints public key

        size_t q_bits;
        q_bits = mpz_sizeinbase(q, 2);
        gmp_printf("q (%lu bits) = %Zd\n", q_bits, q); //prints q bits

        size_t n_bits;
        n_bits = mpz_sizeinbase(n, 2);
        gmp_printf("n (%lu bits) = %Zd\n", n_bits, n); //prints n bits

        size_t d_bits;
        d_bits = mpz_sizeinbase(d, 2);
        gmp_printf("d (%lu bits) = %Zd\n", d_bits, d); //prints d bits

        size_t pq_bits;
        pq_bits = mpz_sizeinbase(pq, 2);
        gmp_printf("pq (%lu bits) = %Zd\n", pq_bits, pq); //prints pq bits
    }

    //Step 9: close all files, clear randomstate, and clear mpz variables
    fclose(pbfile);
    fclose(pvfile);
    randstate_clear();
    mpz_clears(n, p, a, pq, d, NULL);

    return 0;
} //close of the main
