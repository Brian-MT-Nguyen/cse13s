#include "randstate.h"
#include "numtheory.h"
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
    //Intializes input, output, and key files that will be grabbed for program
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
        case 'i': infile = fopen(optarg, "r"); break;
        case 'o': outfile = fopen(optarg, "w"); break;
        case 'n': pbfile = fopen(optarg, "w"); break;
        case 'v': verbose = true; break;
        case 'h': help = true; break;
        default: break;
        }
    }

    //Prints help message if prompted
    if (help) {
        printf("SYNOPSIS\n");
        printf("  Generates an RSA public/private key pair.\n");

        printf("USAGE\n");
        printf("  ./keygen [-hv] [-b bits] -i iters -n pbfile -d pvfile \n");

        printf("OPTIONS\n");
        printf("  -h             Display program usage and help.\n");
        printf("  -v             Display verbose output program.\n");
        printf("  -b bits        Minimum bits needed for public key n.\n");
        printf("  -i iters       Miller-Rabin iterations for testing primes (default: 50).\n");
        printf("  -n pbfile      Public key file (default: rsa.pub).\n");
        printf("  -d pvfile      Private key file (default: rsa. priv).\n");
        printf("  -s seed        Random seed for testing.\n");
    }

    //Creates/opens default write files if pbfile and/or pvfile was not specified
    if (!pb_opened) {
        pbfile = fopen("rsa.pub", "r");
    }

    //Initialize vars that will store public key info
    mpz_t n, exponent, signature;
    mpz_inits(n, exponent, signature, NULL);
    char username[32];
    rsa_read_pub(n, exponent, signature, username, pbfile);

    if (verbose) {
        gmp_printf("user = %s\n", username);
        gmp_printf("s (%zu bits) = %Zd\n", mpz_sizeinbase(signature, 2), signature);
        gmp_printf("n (%zu bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
        gmp_printf("e (%zu bits) = %Zd\n", mpz_sizeinbase(exponent, 2), exponent);
    }

    //Verify the signature
    mpz_t verify_user;
    mpz_init(verify_user);
    mpz_set_str(verify_user, username, 62);
    if (!rsa_verify(verify_user, signature, exponent, n)) {
        fprintf(stderr, "Signature could not be verified. Exiting Program...");
        exit(0);
    }

    rsa_encrypt_file(infile, outfile, n, exponent);

    fclose(pbfile);
    fclose(infile);
    fclose(outfile);
    mpz_clears(n, exponent, signature, verify_user, NULL);
}
