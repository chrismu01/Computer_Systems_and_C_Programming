#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ss.h"

void get_help(void) {
    fprintf(stderr, "SYNOPSIS\n"
                    "   Encrypts data using SS encryption.\n"
                    "   Encrypted data is decrypted by the decrypt program.\n"
                    "\n"
                    "USAGE\n"
                    "   ./encrypt [OPTIONS]\n"
                    "\n"
                    "OPTIONS\n"
                    "   -h              Display program help and usage.\n"
                    "   -v              Display verbose program output.\n"
                    "   -i infile       Input file of data to encrypt (default: stdin).\n"
                    "   -o outfile      Output file for encrypted data (default: stdout).\n"
                    "   -n pbfile       Public key file (default: ss.pub).\n");
}

#define OPTIONS "i:o:n:vh"
int main(int argc, char **argv) {
    bool verbose_checker = false;
    char *pbfile_name = "ss.pub";
    FILE *pbfile;
    FILE *infile = stdin;
    FILE *outfile = stdout;
    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i': //infile
            infile = fopen(optarg, "r");
            if (infile == NULL) { //prints an error message if infile is NULL
                perror("error");
                return EXIT_FAILURE;
            }
            break;

        case 'o': //outfile
            outfile = fopen(optarg, "w");
            if (outfile == NULL) { //prints an error message if outfile is NULL
                perror("error");
                return EXIT_FAILURE;
            }
            break;

        case 'n': //where the public files is at
            pbfile_name = optarg;
            break;

        case 'v': //verbose
            verbose_checker = true;
            break;

        case 'h': //get help case
            get_help();
            return 0;

        default: get_help(); return 0;

        } //closing of the switch cases
    } //closing of the while loop for getopt
    //Step 2: open the public key file
    pbfile = fopen(pbfile_name, "r"); //open the public file as read

    mpz_t n;
    char *username = getenv("USER"); //get username
    mpz_init(n); //initializing variables

    //Step 3: read the public key
    ss_read_pub(n, username, pbfile);

    //Step 4: checking if verbose is enables and printing the following if it is
    if (verbose_checker == true) {
        gmp_printf("user = %s\n", username); //prints username

        size_t n_bits;
        n_bits = mpz_sizeinbase(n, 2);
        gmp_printf("n (%lu bits) = %Zd\n", n_bits, n); //prints public key
    }

    //Step 5: encrypt the file
    ss_encrypt_file(infile, outfile, n);

    //Step 6: close public key file and clear all mpz varables
    fclose(infile);
    fclose(outfile);
    fclose(pbfile);
    mpz_clear(n);

    return 0;
} //closing of main
