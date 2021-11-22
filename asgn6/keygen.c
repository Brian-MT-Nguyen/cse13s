#include "randstate.h"
#include "rsa.h"
#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

#define OPTIONS "b:i:n:d:s:vh"

int main(int argc, char **argv) {
    //Intializes public and private key file that will be grabbed for program
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
            if (pbfile == NULL) {
                fprintf(stderr, "Error creating public key file. Exiting Program.\n");
                fclose(pbfile);
                exit(EXIT_FAILURE);
            }
            break;
        case 'd':
            pv_opened = true;
            pvfile = fopen(optarg, "w");
            if (pvfile == NULL) {
                fprintf(stderr, "Error creating private key file. Exiting Program.\n");
                fclose(pvfile);
                exit(EXIT_FAILURE);
            }
            break;
        case 's': seed = atoi(optarg); break;
        case 'v': verbose = true; break;
        case 'h': help = true; break;
        default: break;
        }
    }

    //Prints help message if prompted or invalid use of arguments
    if (help || optopt != 0) {
        printf("SYNOPSIS\n");
        printf("   Generates an RSA public/private key pair.\n");

        printf("\nUSAGE\n");
        printf("   ./keygen [-hv] [-b bits] -i iters -n pbfile -d pvfile -s seed\n");

        printf("\nOPTIONS\n");
        printf("   -h              Display program help and usage.\n");
        printf("   -v              Display verbose program output.\n");
        printf("   -b bits         Minimum bits needed for public key n.\n");
        printf("   -i iters        Miller-Rabin iterations for testing primes (default: 50).\n");
        printf("   -n pbfile       Public key file (default: rsa.pub).\n");
        printf("   -d pvfile       Private key file (default: rsa. priv).\n");
        printf("   -s seed         Random seed for testing.\n");
        return 0;
    }

    //Creates/opens default write files if pbfile and/or pvfile was not specified
    if (!pb_opened || !pv_opened) {
        if (!pb_opened) {
            pbfile = fopen("rsa.pub", "w");
            if (pbfile == NULL) {
                fprintf(stderr, "Error creating public key file. Exiting program.\n");
                fclose(pbfile);
                exit(EXIT_FAILURE);
            }
        }
        if (!pv_opened) {
            pvfile = fopen("rsa.priv", "w");
            if (pbfile == NULL) {
                fprintf(stderr, "Error creating private key file. Exiting program.\n");
                fclose(pvfile);
                exit(EXIT_FAILURE);
            }
        }
    }

    //Set permissions to private file (only modifiable and accessible by owner)
    fchmod(fileno(pvfile), S_IRUSR | S_IWUSR);

    //Set random state using seed
    randstate_init(seed);

    //Make public key and store info into initialized vars
    mpz_t p, q, n, e;
    mpz_inits(p, q, n, e, NULL);
    rsa_make_pub(p, q, n, e, bits, iters);

    //Make private key and store info into initialized d var
    mpz_t d;
    mpz_init(d);
    rsa_make_priv(d, e, p, q);

    //Get the username of owner
    char *username = getenv("USER");

    //Convert char username to mpz_t
    mpz_t user;
    mpz_init(user);
    mpz_set_str(user, username, 62);

    //Sign username
    mpz_t signature;
    mpz_init(signature);
    rsa_sign(signature, user, d, n);

    //Write keys to respective files
    rsa_write_pub(n, e, signature, username, pbfile);
    rsa_write_priv(n, d, pvfile);

    //Print verbose output if prompted
    if (verbose) {
        printf("user = %s\n", username);
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
