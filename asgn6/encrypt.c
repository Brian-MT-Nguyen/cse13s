#include "rsa.h"
#include <stdio.h>
#include <gmp.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

#define OPTIONS "i:o:n:vh"

int main(int argc, char **argv) {
    //Intializes input, output, and key file that will be grabbed for program
    FILE *infile = stdin;
    FILE *outfile = stdout;
    FILE *pbfile;

    //Initialize variables used to parse through command line
    int opt = 0;
    bool pb_opened = false;
    bool verbose = false;
    bool help = false;

    //Parses through command line input options
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i':
            infile = fopen(optarg, "r");
            if (infile == NULL) {
                fprintf(stderr, "Error opening infile. Exiting Program.\n");
                fclose(infile);
                exit(EXIT_FAILURE);
            }
            break;
        case 'o':
            outfile = fopen(optarg, "w");
            if (outfile == NULL) {
                fprintf(stderr, "Error opening outfile. Exiting Program.\n");
                fclose(outfile);
                exit(EXIT_FAILURE);
            }
            break;
        case 'n':
            pb_opened = true;
            pbfile = fopen(optarg, "r");
            if (pbfile == NULL) {
                fprintf(stderr, "Error opening public key file. Exiting program.\n");
                fclose(pbfile);
                exit(EXIT_FAILURE);
            }
            break;
        case 'v': verbose = true; break;
        case 'h': help = true; break;
        default: break;
        }
    }

    //Prints help message if prompted or invalid use of arguments
    if (help || optopt != 0) {
        printf("SYNOPSIS\n");
        printf("   Encrypts data using RSA encryption.\n");
        printf("   Encrypted data is decrypted by the decrypt program.\n");

        printf("\nUSAGE\n");
        printf("   ./encrypt [-hv] [-i infile] [-o outfile] -n pubkey\n");

        printf("\nOPTIONS\n");
        printf("   -h              Display program help and usage.\n");
        printf("   -v              Display verbose program output.\n");
        printf("   -i infile       Input file of data to encrypt (default: stdin).\n");
        printf("   -o outfile      Output file for encrypted data (default: stdout).\n");
        printf("   -n pbfile       Public key file (default: rsa.pub).\n");
        return 0;
    }
    //Creates/opens default write file if pbfile was not specified
    if (!pb_opened) {
        pbfile = fopen("rsa.pub", "r");
    }

    //Read public key and store info into initialized vars
    mpz_t n, exponent, signature;
    mpz_inits(n, exponent, signature, NULL);
    char username[32];
    rsa_read_pub(n, exponent, signature, username, pbfile);

    //Print verbose output if prompted
    if (verbose) {
        printf("user = %s\n", username);
        gmp_printf("s (%zu bits) = %Zd\n", mpz_sizeinbase(signature, 2), signature);
        gmp_printf("n (%zu bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
        gmp_printf("e (%zu bits) = %Zd\n", mpz_sizeinbase(exponent, 2), exponent);
    }

    //Verify the signature
    mpz_t verify_user;
    mpz_init(verify_user);
    mpz_set_str(verify_user, username, 62);
    if (!rsa_verify(verify_user, signature, exponent, n)) {
        fprintf(stderr, "Signature could not be verified. Exiting Program.\n");
        exit(EXIT_FAILURE);
    }

    //Encrypt file if signature is verified
    rsa_encrypt_file(infile, outfile, n, exponent);

    //Close files and clear vars
    fclose(pbfile);
    fclose(infile);
    fclose(outfile);
    mpz_clears(n, exponent, signature, verify_user, NULL);
}
