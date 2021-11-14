#include "numtheory.h"
#include "randstate.h"
#include <gmp.h>

//Finds the greatest common divisor between a and b
//
//d: the divisor
//a: the first integer
//b: the second integer
void gcd(mpz_t d, mpz_t a, mpz_t b) {
    mpz_t temp;
    mpz_init(temp);
    while (mpz_sgn(b) != 0) {
        mpz_set(temp, b);
        mpz_mod(b, a, b);
        mpz_set(a, temp);
    }
    mpz_set(d, a);
    mpz_clear(temp);
}

//Finds the modular multiplicative inverse of n using a and stores in i
//
//i: the modular inverse of a
//a: the number
//n: the modulus number
//
//DESCRIPTION NEEDS WORK
void mod_inverse(mpz_t i, mpz_t a, mpz_t n) {
    //Initialize r and r-prime from algorithm
    mpz_t r, r_prime;
    mpz_init_set(r, n);
    mpz_init_set(r_prime, a);

    //Initialize i and i_prime (in place of t and t_prime from algorithm)
    mpz_t i_prime;
    mpz_set_ui(i, 0);
    mpz_init_set_ui(i_prime, 1);

    //Intiailize q from algorithm
    mpz_t q;
    mpz_init(q);

    //Initialize temp variable to fake parrallel assignment
    mpz_t temp;
    mpz_init(temp);
    while (mpz_sgn(r) != 0) {
        mpz_fdiv_q(q, r, r_prime);

        mpz_set(temp, r);
        mpz_set(r, r_prime);
        mpz_submul(temp, q, r_prime);
        mpz_set(r_prime, temp);

        mpz_set(temp, i);
        mpz_set(i, i_prime);
        mpz_submul(temp, q, i_prime);
        mpz_set(i_prime, temp);
    }

    if (mpz_cmp_ui(r, 1) > 0) {
        mpz_set_ui(i, 0);
    }

    if (mpz_sgn(i) == -1) {
        mpz_add(i, i, n);
    }

    mpz_clears(r, r_prime, i_prime, q, temp);
}

//Computes base^exponent mod modulus, then puts value in out
//
//out: output of the computed value
//base: number that is the base value
//exponent: number that is the exponent of the base
//modulus: number that is be used to mod value
void pow_mod(mpz_t out, mpz_t base, mpz_t exponent, mpz_t modulus) {
    //Initialize variable that holds base which will change in function
    mpz_t changing_base;

    //Initialize variable to check if exponent is odd
    mpz_t odd_checker;
    mpz_init(odd_checker);

    mpz_set_ui(out, 1);
    mpz_init_set(changing_base, base);

    while (mpz_sgn(exponent) == 1) {
        if (mpz_mod_ui(odd_checker, exponent, 2) == 1) {
            mpz_mul(out, out, changing_base);
            mpz_mod(out, out, modulus);
        }
        mpz_mul(changing_base, changing_base, changing_base);
        mpz_mod(changing_base, changing_base, modulus);
        mpz_fdiv_q_ui(exponent, exponent, 2);
    }
    mpz_clears(changing_base, odd_checker);
}
//Tests if a number is prime via the Miller-Rabin Algorithm
//
//n: the number to test if prime or not
//iters: amount of iterations to test prime number with
bool is_prime(mpz_t n, uint64_t iters) {
    //Initialize  s and r from n
    mpz_t s, r;
    mpz_inits(s, r);
    mpz_sub_ui(r, n, 1);

    //Initialize modulus checker to see if n is even
    mpz_t mod_checker;
    mpz_init(mod_checker);
    //Check if n is even or two
    if ((mpz_cmp_ui(n, 2) == 0) || mpz_mod_ui(mod_checker, n, 2) == 0) {
        return false;
    }
    //Divides r until r is odd and increments s, satisfying equation from pseudo
    while (mpz_even_p(r) != 0) {
        mpz_fdiv_q_ui(r, r, 2);
        mpz_add_ui(s, s, 1);
    }

    //Initialize variables that generates random a in range two to n-two
    mpz_t range, a;
    mpz_inits(range, a);
    mpz_sub_ui(range, n, 3);

    //Initialize variables used in algorithm
    mpz_t y, j, two, conditional_n, conditional_s;
    mpz_inits(y, j, two, conditional_n, conditional_s);
    mpz_sub_ui(conditional_n, n, 1);
    mpz_sub_ui(conditional_s, s, 1);
    mpz_set_ui(two, 2);
    for (uint64_t i = 0; i <= iters; i++) {
        mpz_urandomm(a, state, range);
        mpz_add_ui(a, a, 2);
        pow_mod(y, a, r, n);
        if ((mpz_cmp_ui(y, 1) != 0) && (mpz_cmp(y, conditional_n) != 0)) {
            mpz_set_ui(j, 1);
            while ((mpz_cmp(j, conditional_s) <= 0) && (mpz_cmp(y, conditional_n) != 0)) {
                pow_mod(y, y, two, n);
            }
            if (mpz_cmp_ui(y, 1) == 0) {
                return false;
            }
            mpz_add_ui(j, j, 1);
            if (mpz_cmp(y, conditional_n) != 0) {
                return false;
            }
        }
    }
    mpz_clears(s, r, a, range, y, j, two, conditional_n, conditional_s);
    return true;
}
//Generates a number that is bits long, then tests if prime using is_prime
//
//p: the stored prime number
//bits: value of how large the number must be bit wise
//iters: the amount of iterations to test if a number is prime
void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
    //Make sure p isn't prime to generate new one
    mpz_set_ui(p, 4);

    while (!is_prime(p, iters)) {
        mpz_rrandomb(p, state, bits);
        if (is_prime(p, iters)) {
            break;
        }
    }
}
