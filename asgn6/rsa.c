#include "rsa.h"
#include "randstate.h"
#include "numtheory.h"

void rsa_make_pub(mpz_t p, mpz_t q, mpz_t n, mpz_t e, uint64_t nbits, uint64_t iters) {
    //Initialize range for p_bits before adding
    mpz_t p_bits, p_range;
    mpz_init2(p_bits, nbits);
    mpz_init2(p_range, nbits);
    mpz_set_ui(p_range, nbits);
    mpz_mul_ui(p_range, p_range, 2);
    mpz_fdiv_q_ui(p_range, p_range, 4);

    //Generate range for bits to p
    mpz_urandomm(p_bits, state, p_range);

    //Add nbits/four to range to satisfying range
    mpz_set_ui(p_range, nbits);
    mpz_fdiv_q_ui(p_range, p_range, 4);
    mpz_add(p_bits, p_bits, p_range);
    mpz_set(p, p_bits);

    uint64_t p_bits_ui = mpz_get_ui(p_bits);

    //Generate p from p_bits
    make_prime(p, p_bits_ui, iters);

    //Initialize q_bits getting remaining bits for p_bits
    mpz_t q_bits;
    mpz_init2(q_bits, nbits);
    mpz_ui_sub(q_bits, nbits, p_bits);

    uint64_t q_bits_ui = mpz_get_ui(q_bits);
    //Generate q from n_bits - p_bits
    make_prime(q, q_bits_ui, iters);

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
    mpz_clears(p_bits, p_range, q_bits, totient_n, p_one, q_one, coprime_checker, NULL);
}

void rsa_write_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    gmp_fprintf(pbfile, "%x\n", n);
    gmp_fprintf(pbfile, "%x\n", e);
    gmp_fprintf(pbfile, "%x\n", s);
    gmp_fprintf(pbfile, "%s\n", username);
}

void rsa_read_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    gmp_fscanf(pbfile, "%x\n", n);
    gmp_fscanf(pbfile, "%x\n", e);
    gmp_fscanf(pbfile, "%x\n", s);
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
    gmp_fprintf(pvfile, "%x\n", n);
    gmp_fprintf(pvfile, "%x\n", d);
}

void rsa_read_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    gmp_fscanf(pvfile, "%x\n", n);
    gmp_fscanf(pvfile, "%x\n", d);
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
