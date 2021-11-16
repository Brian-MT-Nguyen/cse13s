#include "rsa.h"
#include "randstate.h"
#include "numtheory.h"
#include <stdlib.h>

void rsa_make_pub(mpz_t p, mpz_t q, mpz_t n, mpz_t e, uint64_t nbits, uint64_t iters) {
    //Range from lower to upper: number = (random() % (upper - lower + 1)) + lower
    //p_bits range is nbits/four to (three*nbits)/four
    uint64_t p_bits = (random() % (((3 * nbits) / 4) - (nbits / 4) + 1)) + (nbits / 4);

    //Add one to p_bits to make sure n is greater or equal to nbits
    p_bits += 1;

    //Generate p from p_bits
    make_prime(p, p_bits, iters);

    //q_bits is remaining bits from nbits not taken by p_bits
    uint64_t q_bits = nbits - p_bits;

    //Add one to q_bits to make sure n is greater or equal to nbits
    q_bits += 1;

    //Generate q from n_bits - p_bits
    make_prime(q, q_bits, iters);

    //Calculate n which is p * q
    mpz_mul(n, p, q);

    //Initialize totient_n being (p-one)*(q-one)
    mpz_t totient_n, p_one, q_one;
    mpz_inits(totient_n, p_one, q_one, NULL);
    mpz_sub_ui(p_one, p, 1);
    mpz_sub_ui(q_one, q, 1);

    //Calculate the totient of n
    mpz_mul(totient_n, p_one, q_one);

    //Initialize coprime checker that gets output of gcd
    mpz_t coprime_checker;
    mpz_init(coprime_checker);

    //Generate random numbers for e where the gcd of totient_n and e are one making them coprime
    while (mpz_cmp_ui(coprime_checker, 1) != 0) {
        mpz_urandomb(e, state, nbits);
        gcd(coprime_checker, e, totient_n);
    }
    //Clear mpz vars
    mpz_clears(totient_n, p_one, q_one, coprime_checker, NULL);
}

void rsa_write_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    gmp_fprintf(pbfile, "%Zx\n", n);
    gmp_fprintf(pbfile, "%Zx\n", e);
    gmp_fprintf(pbfile, "%Zx\n", s);
    gmp_fprintf(pbfile, "%s\n", username);
}

void rsa_read_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    gmp_fscanf(pbfile, "%Zx\n", n);
    gmp_fscanf(pbfile, "%Zx\n", e);
    gmp_fscanf(pbfile, "%Zx\n", s);
    gmp_fscanf(pbfile, "%s\n", username);
}

void rsa_make_priv(mpz_t d, mpz_t e, mpz_t p, mpz_t q) {
    //Initialize totient_n being (p-one)*(q-one)
    mpz_t totient_n, p_one, q_one;
    mpz_inits(totient_n, p_one, q_one, NULL);
    mpz_sub_ui(p_one, p, 1);
    mpz_sub_ui(q_one, q, 1);

    //Calculate the totient of n
    mpz_mul(totient_n, p_one, q_one);
    mod_inverse(d, e, totient_n);

    mpz_clears(totient_n, p_one, q_one, NULL);
}

void rsa_write_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    gmp_fprintf(pvfile, "%Zx\n", n);
    gmp_fprintf(pvfile, "%Zx\n", d);
}

void rsa_read_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    gmp_fscanf(pvfile, "%Zx\n", n);
    gmp_fscanf(pvfile, "%Zx\n", d);
}

void rsa_encrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n) {
    pow_mod(c, m, e, n);
}

void rsa_encrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t e);

void rsa_decrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t n) {
    pow_mod(m, c, d, n);
}

void rsa_decrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t d);

void rsa_sign(mpz_t s, mpz_t m, mpz_t d, mpz_t n) {
    pow_mod(s, m, d, n);
}

bool rsa_verify(mpz_t m, mpz_t s, mpz_t e, mpz_t n) {
    //Initialize variable that stores inversed signature
    mpz_t t;
    mpz_init(t);

    pow_mod(t, s, e, n);
    if (mpz_cmp(t, m) == 0) {
        mpz_clear(t);
        return true;
    }
    mpz_clear(t);
    return false;
}
