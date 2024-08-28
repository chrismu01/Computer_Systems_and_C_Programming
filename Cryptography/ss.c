#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "ss.h"
#include "randstate.h"
#include "numtheory.h"
#include <inttypes.h>

//make pub function function
void ss_make_pub(mpz_t p, mpz_t q, mpz_t n, uint64_t nbits, uint64_t iters) {

    //init variables
    mpz_t temp_p, temp_q, pq, result_p, result_q, verify_p, verify_q;
    mpz_inits(temp_p, temp_q, pq, result_p, result_q, verify_p, verify_q, NULL);

    uint64_t temp_nbits = nbits; //temping nbits (to not accidentally overwrite it)
    uint64_t temp_iters = iters; //temping iters (to not accidentally overwrite it)

    uint64_t temp_bits = temp_nbits / 5; //temp bits for p (min bits)
    uint64_t numerator = (temp_nbits * 2);
    uint64_t temp1_bits = (numerator / 5); //max bits

    uint64_t diff = (temp1_bits - temp_bits);

    uint64_t pbits = (random() % (temp1_bits)) % diff; //set p bits
    uint64_t qbits = (temp_nbits - (pbits * 2)); //setting qbits

    //make prime numbers
    make_prime(p, pbits + 51, temp_iters);
    make_prime(q, qbits - 100, temp_iters);

    //verify p % q-1
    mpz_sub_ui(result_p, q, 1);
    mpz_set(verify_p, p);

    mpz_sub_ui(result_q, p, 1);
    mpz_set(verify_q, q);

    //while verify p is 0 and verify q is 0
    //ben the tutor recommended to use mpz_divisible()
    while ((mpz_divisible_p(verify_q, result_q)) | (mpz_divisible_p(verify_q, result_q))) {
        make_prime(p, pbits, iters);
        make_prime(q, qbits, iters);

        mpz_sub_ui(result_p, q, 1);
        mpz_mod(verify_p, q, result_p);

        mpz_sub_ui(result_q, p, 1);
        mpz_mod(verify_q, p, result_q);
    }
    mpz_mul(pq, p, q);
    mpz_mul(pq, pq, p); // calcualting p squared times q

    mpz_set(n, pq); //setting n (result)

    //clearing variables
    mpz_clears(temp_p, temp_q, pq, result_p, result_q, verify_p, verify_q, NULL);
}

//ss make private function
void ss_make_priv(mpz_t d, mpz_t pq, const mpz_t p, const mpz_t q) {
    //initializing variables
    mpz_t temp_p, temp_q, numerator, denominator, result, temp_n, n, temp_pminus, temp_qminus;
    mpz_inits(
        temp_p, temp_q, numerator, denominator, result, temp_n, n, temp_pminus, temp_qminus, NULL);
    mpz_set(temp_p, p); //setting temp variable for p
    mpz_set(temp_q, q); //setting temo variable for q

    mpz_sub_ui(temp_pminus, temp_p, 1); //calcualte p - 1
    mpz_sub_ui(temp_qminus, temp_q, 1); //calculate q - 1

    mpz_mul(pq, p, q); //calculate pq

    //mpz_set(n,pq); //setting n

    mpz_mul(numerator, temp_pminus, temp_qminus); //calculating numerator
    gcd(denominator, temp_pminus, temp_qminus); //calculating denominator
    mpz_fdiv_q(result, numerator, denominator); //divide it

    mpz_mul(temp_n, p, q);
    mpz_mul(temp_n, temp_n, p);
    mpz_set(n, temp_n);
    mod_inverse(d, n, result);

    //clear variables used
    mpz_clears(
        temp_p, temp_q, numerator, denominator, result, temp_n, n, temp_pminus, temp_qminus, NULL);
}

//ss write public
void ss_write_pub(const mpz_t n, const char username[], FILE *pbfile) {
    gmp_fprintf(pbfile, "%Zx\n", n); //prints n into public file
    gmp_fprintf(pbfile, "%s\n", username); //prints username in public file
}

//ss write private function
void ss_write_priv(const mpz_t pq, const mpz_t d, FILE *pvfile) {
    gmp_fprintf(pvfile, "%Zx\n", pq); //prints pq into private file
    gmp_fprintf(pvfile, "%Zx\n", d); //prints d into private file
}

//ss read public file
void ss_read_pub(mpz_t n, char username[], FILE *pbfile) {
    gmp_fscanf(pbfile, "%Zx\n", n); //read for the n in public file
    gmp_fscanf(pbfile, "%s\n", username); //read for the username in the public file
}

//ss read private function
void ss_read_priv(mpz_t pq, mpz_t d, FILE *pvfile) {
    gmp_fscanf(pvfile, "%Zx\n", pq); //read for the pq in the private file
    gmp_fscanf(pvfile, "%Zx\n", d); //read for the d in the private file
}

//ss encrypt function
void ss_encrypt(mpz_t c, const mpz_t m, const mpz_t n) {
    pow_mod(c, m, n, n); //use powmod function
}

//ss encrypt file function
void ss_encrypt_file(FILE *infile, FILE *outfile, const mpz_t n) {

    //Step 1: calculate the block size k
    //k = floor((log2(sqrt(n)) - 1) / 8)
    mpz_t m, c;
    mpz_inits(m, c, NULL);
    size_t k;
    k = mpz_sizeinbase(n, 2);
    k = (k / 2);
    k = (k - 1);
    k = (k / 8);

    //Step 2: Dynamically allocate an array that can hold k bytes
    uint8_t *arr1 = (uint8_t *) calloc(k, sizeof(uint8_t));

    //Step 3: set the 0th byte to the block to 0xFF
    arr1[0] = 0xFF;

    //Step 4: while there are still unprocessed bytes in infile:
    size_t j;

    do {
        j = fread(&arr1[1], sizeof(uint8_t), k - 1, infile);
        mpz_import(m, j + 1, 1, sizeof(uint8_t), 1, 0, arr1);
        ss_encrypt(c, m, n);
        gmp_fprintf(outfile, "%Zx\n", c); //prints c into the outfile

    } while (!feof(infile));

    //free array and clear variables
    free(arr1);
    mpz_clears(m, c, NULL);
}

// ss decrypt function
void ss_decrypt(mpz_t m, const mpz_t c, const mpz_t d, const mpz_t pq) {
    pow_mod(m, c, d, pq); //use of pow_mod from numtheory.c
}

//ss decrypt file function
void ss_decrypt_file(FILE *infile, FILE *outfile, const mpz_t d, const mpz_t pq) {
    //init variables
    mpz_t c, m;
    mpz_inits(c, m, NULL);
    //calculate k
    size_t k;
    k = mpz_sizeinbase(pq, 2);
    k = (k - 1);
    k = (k / 8);

    //allocate memory for array
    uint8_t *arr2 = (uint8_t *) calloc(k, sizeof(uint8_t));
    size_t j;

    //running the code untike the end of infile
    do {
        gmp_fscanf(infile, "%Zx\n", c);
        ss_decrypt(m, c, d, pq);
        mpz_export(arr2, &j, 1, (sizeof(uint8_t)), 1, 0, m);
        fwrite(arr2 + 1, sizeof(uint8_t), j - 1, outfile);
    } while (!feof(infile));

    //free the array and variables
    free(arr2);
    mpz_clears(c, m, NULL);
}
