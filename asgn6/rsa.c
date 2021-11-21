#include "rsa.h"
#include "randstate.h"
#include "numtheory.h"
#include <stdlib.h>

//Generates a public key for the user made of random prime (p), random prime (q), product of p*q (n), and random public exponent (e)
//
//p: the first random prime number
//q: the second random prime number
//n: the product of p and q
//e: the public exponent computed by gcd of totient of n and random e equaling one
void rsa_make_pub(mpz_t p, mpz_t q, mpz_t n, mpz_t e, uint64_t nbits, uint64_t iters) {
    //Range from lower to upper: number = (random() % (upper - lower + 1)) + lower
    //p_bits range is nbits/four to (three*nbits)/four
    uint64_t p_bits = (random() % (((3 * nbits) / 4) - (nbits / 4) + 1)) + (nbits / 4);

    //q_bits is remaining bits from nbits not taken by p_bits
    uint64_t q_bits = nbits - p_bits;

    //Add one to p_bits and q_bits to make sure n is greater or equal to nbits
    p_bits += 1;
    q_bits += 1;

    //Generate p and q prime numbers
    make_prime(p, p_bits, iters);
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

//Writes the generated public key to the pbfile in specific formatted output
//
//n: the product of p and q (from rsa_make_pub)
//e: the public exponent (from rsa_make_pub)
//s: the signature (from rsa_sign)
//username: the user's username in linux
//pbfile: the file to write to
void rsa_write_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    gmp_fprintf(pbfile, "%Zx\n", n);
    gmp_fprintf(pbfile, "%Zx\n", e);
    gmp_fprintf(pbfile, "%Zx\n", s);
    fprintf(pbfile, "%s\n", username);
}

//Reads the generated public key from the pbfile in specific formatted input
//
//n: the product of p and q (from rsa_make_pub)
//e: the public exponent (from rsa_make_pub)
//s: the signature (from rsa_sign)
//username: the user's username in linux
//pbfile: the file to read from
void rsa_read_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    gmp_fscanf(pbfile, "%Zx\n", n);
    gmp_fscanf(pbfile, "%Zx\n", e);
    gmp_fscanf(pbfile, "%Zx\n", s);
    fscanf(pbfile, "%s\n", username);
}

//Generates the private key based on p, q, e from rsa_make_pub and stores in d
//
//d: the variable to store the private key
//e: the public exponent (from rsa_make_pub)
//p: the first random prime (from rsa_make_pub)
//q: the second random prime (from rsa_make_pub)
void rsa_make_priv(mpz_t d, mpz_t e, mpz_t p, mpz_t q) {
    //Initialize totient_n being (p-one)*(q-one)
    mpz_t totient_n, p_one, q_one;
    mpz_inits(totient_n, p_one, q_one, NULL);
    mpz_sub_ui(p_one, p, 1);
    mpz_sub_ui(q_one, q, 1);

    //Calculate the totient of n
    mpz_mul(totient_n, p_one, q_one);

    //Makes the private key d from e and totient n (mod_inverse)
    mod_inverse(d, e, totient_n);

    mpz_clears(totient_n, p_one, q_one, NULL);
}

//Writes the generated private key to pvfile in specific formatted output
//
//n: the product of p and q (from rsa_make_pub)
//d: the stored private key (from rsa_make_priv)
//pvfile: the file to write to
void rsa_write_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    gmp_fprintf(pvfile, "%Zx\n", n);
    gmp_fprintf(pvfile, "%Zx\n", d);
}

//Reads the generated private key from the pvfile in specific formatted input
//
//n: the product of p and q (from rsa_make_pub)
//d: the stored private key (from rsa_make_priv)
//pvfile: the file to read from
void rsa_read_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    gmp_fscanf(pvfile, "%Zx\n", n);
    gmp_fscanf(pvfile, "%Zx\n", d);
}

//Encrypts message and stores in var c using pow_mod from numtheory
//
//c: the variable to store the encrypted message
//m: the original message to be encrypted
//e: the public exponent (from rsa_make_pub)
//n: the product of p and q (from rsa_make_pub)
void rsa_encrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n) {
    pow_mod(c, m, e, n);
}

//Encrypts the infile in k sized blocks at a time and writes to outfile in hexstrings
//
//infile: the input file to encrypt from
//outfile: the output file to encrypt to
//n: the product of p and q (from rsa_make_pub)
//e: the public exponent (from rsa_make_pub)
void rsa_encrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t e) {
    //Set k to block size that will be read
    size_t k = (mpz_sizeinbase(n, 2) - 1) / 8;
    uint8_t *block = (uint8_t *) calloc(k, sizeof(uint8_t));

    //Prepend padding at byte zero to avoid impossible cases
    block[0] = 0xFF;

    //Initialize m which will store the data of the read file
    mpz_t m;
    mpz_init(m);

    //Reads from binary file starting index one to k-one (j is return value of total bits written)
    size_t j;
    while ((j = fread(block + 1, sizeof(uint8_t), k - 1, infile)) > 0) {
        //Convert the bytes read from file into m (including the 0th byte)
        mpz_import(m, j + 1, 1, sizeof(uint8_t), 1, 0, block);

        //Encrypt the read message and write it to outfile
        rsa_encrypt(m, m, e, n);
        gmp_fprintf(outfile, "%Zx\n", m);
    }
    //Free/Clear memory
    free(block);
    mpz_clear(m);
}

//Decrypts message and stores in var m using pow_mod from numtheory
//
//m: the variable that stores the decrypted message (the original one)
//c: the encrypted message
//d: the stored private key (from rsa_make_priv)
//n: the product of p and q (from rsa_make_pub)
void rsa_decrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t n) {
    pow_mod(m, c, d, n);
}

//Decrypts the infile by scaning hexstrings and placing it in k sized blocks used to write the decrypted file to outfile
//
//infile: the input file to decrypt from
//outfile: the output file to decrypt to
//n: the product of p and q (from rsa_make_pub)
//d: the stored private key (from rsa_make_priv)
void rsa_decrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t d) {
    //Set k to block size that will be read
    size_t k = (mpz_sizeinbase(n, 2) - 1) / 8;
    uint8_t *block = (uint8_t *) calloc(k, sizeof(uint8_t));

    //Initialize c which stores scanned lines from infile
    mpz_t c;
    mpz_init(c);

    //Reads from binary file starting index one to k-one (j is store variable of total bits written from export)
    size_t j;
    while (gmp_fscanf(infile, "%Zx\n", c) > 0) {
        //Decrypt each scan from c
        rsa_decrypt(c, c, d, n);

        //Convert the bytes read from c to block and puts bytes read in j
        mpz_export(block, &j, 1, sizeof(uint8_t), 1, 0, c);

        //Write from block one to j - one to outfile
        fwrite(block + 1, sizeof(uint8_t), j - 1, outfile);
    }
    //Free/Clear memory
    free(block);
    mpz_clear(c);
}

//Signs the message and stores in var s using pow_mod in numtheory
//
//s: the variable to store the signature
//m: the message to sign
//d: the stored private key (from rsa_make_priv)
//n: the product of p and q (from rsa_make_pub)
void rsa_sign(mpz_t s, mpz_t m, mpz_t d, mpz_t n) {
    pow_mod(s, m, d, n);
}

//Verifies the signed message using an expected message m using pow_mod in numtheory
//Returns true if the signature and expected message are identical
//Returns flase if the signature and expected message are not identical
//
//m: the expected message used to verify the signature
//s: the stored signature (from rsa_sign)
//e: the public exponent (from rsa_make_pub)
//n: the product of p and q (from rsa_make_pub)
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
