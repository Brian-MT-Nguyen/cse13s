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

#define OPTIONS "b:i:n:d:s:vh"

int main(int argc, char **argv) {
    //Intializes input and output files that will be grabbed for program
    FILE *pbfile;
    FILE *pvfile;

    //Initialize variables used to parse through command line
    int opt = 0;
    uint64_t bits = 256;
    uint64_t iters = 50;
    uint64_t seed = time(NULL);
    bool pb_opened = false;
    bool pv_opened = false;
    bool verbose = false;
    bool help = false;

    //Parses through command line input options
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'b': bits = atoi(optarg); break;
        case 'i': iters = atoi(optarg); break;
        case 'n':
            pb_opened = true;
            pbfile = fopen(optarg, "w");
            break;
        case 'd':
            pv_opened = true;
            pvfile = fopen(optarg, "w");
            break;
        case 's': seed = atoi(optarg); break;
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
    if (!pb_opened || !pv_opened) {
        if (!pb_opened) {
            pbfile = fopen("rsa.pub", "w");
        }
        if (!pv_opened) {
            pvfile = fopen("rsa.priv", "w");
        }
    }

    //Set permissions to private file (only modifiable and accessible by owner)
    fchmod(fileno(pvfile), S_IRUSR | S_IWUSR);

    //Set random state using seed
    randstate_init(seed);

    //Initialize p, q, n, e to use to make the public key
    mpz_t p, q, n, e;
    mpz_inits(p, q, n, e, NULL);
    rsa_make_pub(p, q, n, e, bits, iters);

    //Initialize d to store new made private key
    mpz_t d;
    mpz_init(d);
    rsa_make_priv(d, e, p, q);

    //Get the username of owner
    char *username = getenv("USER");

    //Convert char to mpz_t
    mpz_t user;
    mpz_init(user);
    mpz_set_str(user, username, 62);

    //Sign username
    mpz_t signature;
    mpz_init(signature);
    rsa_sign(signature, user, d, n);

    //Write Keys to respective files
    rsa_write_pub(n, e, signature, username, pbfile);
    rsa_write_priv(n, d, pvfile);

    //If Verbose is inputted print output
    if (verbose) {
        gmp_printf("user = %s\n", username);
        gmp_printf("s (%zu bits) = %Zd\n", mpz_sizeinbase(signature, 2), signature);
        gmp_printf("p (%zu bits) = %Zd\n", mpz_sizeinbase(p, 2), p);
        gmp_printf("q (%zu bits) = %Zd\n", mpz_sizeinbase(q, 2), q);
        gmp_printf("n (%zu bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
        gmp_printf("e (%zu bits) = %Zd\n", mpz_sizeinbase(e, 2), e);
        gmp_printf("d (%zu bits) = %Zd\n", mpz_sizeinbase(d, 2), d);
    }

    //Close files and free vars/state
    fclose(pbfile);
    fclose(pvfile);
    mpz_clears(p, q, n, e, d, user, signature, NULL);
    randstate_clear();
}
