#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ss.h"
#include <inttypes.h>

void get_help(void) {
    fprintf(stderr, "SYNOPSIS\n"
                    "Decrypts data using SS decryption.\n"
                    "Encrypted data is encrypted by the encrypt program.\n"
                    "\n"
                    "USAGE\n"
                    "   ./decrypt [OPTIONS]\n"
                    "\n"
                    "OPTIONS\n"
                    "   -h              Display program help and usage.\n"
                    "   -v              Display verbose program output.\n"
                    "   -i infile       Input file of data to decrypt (default: stdin).\n"
                    "   -o outfile      Output file for decrypted data (default: stdout).\n"
                    "   -n pvfile       Private key file (default: ss.priv).\n");
}

#define OPTIONS "i:o:n:vh"
int main(int argc, char **argv) {
    FILE *infile = stdin;
    FILE *outfile = stdout;
    FILE *pvfile;
    char *pvfile_name = "ss.priv";
    bool verbose_checker = false;
    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i': //input file to decrypt
            infile = fopen(optarg, "r");
            if (infile == NULL) { //prints error message if error opening file

                perror("error");
                return EXIT_FAILURE;
            }

            break;

        case 'o': //output file to decrypt
            outfile = fopen(optarg, "w");
            if (outfile == NULL) { //prints error message if error opening file
                perror("error");
                return EXIT_FAILURE;
            }

            break;

        case 'n': //file containing private key
            pvfile_name = optarg;
            break;

        case 'v': //prints the verbose
            verbose_checker = true;
            break;

        case 'h': //help function
            get_help();
            return 0;

        default: get_help(); return 0;

        } //closing of switch case
    } //closing of while loop

    //Step 2: opening files
    pvfile = fopen(pvfile_name, "r");

    //Step 3: read private key
    mpz_t pq, d;
    mpz_inits(pq, d, NULL);
    ss_read_priv(pq, d, pvfile);

    //Step 4: checking if verbose is enables and printing the following if it is
    if (verbose_checker == true) {
        size_t pq_bits;
        pq_bits = mpz_sizeinbase(pq, 2);
        gmp_printf("pq (%lu bits) = %Zd\n", pq_bits, pq);

        size_t d_bits;
        d_bits = mpz_sizeinbase(d, 2);
        gmp_printf("d (%lu bits) = %Zd\n", d_bits, d);
    }

    //Step 5: Decrypt the file
    ss_decrypt_file(infile, outfile, d, pq);

    //Step 6: close files and clear mpz variables
    fclose(infile);
    fclose(outfile);
    fclose(pvfile);
    mpz_clears(pq, d, NULL);

    return 0;
} //closing of main
