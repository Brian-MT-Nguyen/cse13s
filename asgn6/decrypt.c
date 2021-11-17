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
    FILE *pvfile;
    //Initialize variables used to parse through command line
    int opt = 0;
    bool pv_opened = false;
    bool verbose = false;
    bool help = false;

    //Parses through command line input options
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i': infile = fopen(optarg, "r"); break;
        case 'o': outfile = fopen(optarg, "w"); break;
        case 'n':
            pv_opened = true;
            pvfile = fopen(optarg, "r");
            break;
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

    //Creates/opens default write files if pvfile was not specified
    if (!pv_opened) {
        pvfile = fopen("rsa.priv", "r");
    }

    //Initialize vars that will store private key info
    mpz_t n, priv_key;
    mpz_inits(n, priv_key, NULL);
    rsa_read_priv(n, priv_key, pvfile);

    if (verbose) {
        gmp_printf("n (%zu bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
        gmp_printf("e (%zu bits) = %Zd\n", mpz_sizeinbase(priv_key, 2), priv_key);
    }

    rsa_decrypt_file(infile, outfile, n, priv_key);

    fclose(pvfile);
    fclose(infile);
    fclose(outfile);
    mpz_clears(n, priv_key, NULL);
}
