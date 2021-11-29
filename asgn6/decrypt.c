#include "rsa.h"
#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

#define OPTIONS "i:o:n:vh"

int main(int argc, char **argv) {
    //Intializes input, output, and key file that will be grabbed for program
    FILE *infile = stdin;
    FILE *outfile = stdout;
    FILE *pvfile;

    //Initialize variables used to parse through command line
    int opt = 0;
    bool pv_opened = false;
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
                fprintf(stderr, "Error creating outfile. Exiting Program.\n");
                fclose(outfile);
                exit(EXIT_FAILURE);
            }
            break;
        case 'n':
            pv_opened = true;
            pvfile = fopen(optarg, "r");
            if (pvfile == NULL) {
                fprintf(stderr, "Error opening private key file. Exiting Program.\n");
                fclose(pvfile);
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
        printf("   Decrypts data using RSA decryption.\n");
        printf("   Encrypted data is encrypted by the encrypt program.\n");

        printf("\nUSAGE\n");
        printf("   ./decrypt [-hv] [-i infile] [-o outfile] -d privkey\n");

        printf("\nOPTIONS\n");
        printf("   -h              Display program help and usage.\n");
        printf("   -v              Display verbose program output.\n");
        printf("   -i infile       Input file of data to encrypt (default: stdin).\n");
        printf("   -o outfile      Output file for encrypted data (default: stdout).\n");
        printf("   -d pvfile       Private key file (default: rsa.priv).\n");
        return 0;
    }

    //Creates/opens default write file if pvfile was not specified
    if (!pv_opened) {
        pvfile = fopen("rsa.priv", "r");
        if (pvfile == NULL) {
            fprintf(stderr, "Error opening private key file. Exiting program.\n");
            exit(EXIT_FAILURE);
        }
    }

    //Read private key and store info into initialized vars
    mpz_t n, priv_key;
    mpz_inits(n, priv_key, NULL);
    rsa_read_priv(n, priv_key, pvfile);

    //Print verbose output if prompted
    if (verbose) {
        gmp_printf("n (%zu bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
        gmp_printf("e (%zu bits) = %Zd\n", mpz_sizeinbase(priv_key, 2), priv_key);
    }

    //Decrypt file
    rsa_decrypt_file(infile, outfile, n, priv_key);

    //Close files and clear vars
    fclose(pvfile);
    fclose(infile);
    fclose(outfile);
    mpz_clears(n, priv_key, NULL);
}
