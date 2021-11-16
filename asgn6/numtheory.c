#include "numtheory.h"
#include "randstate.h"

//Finds the greatest common divisor between a and b
//
//d: the output (common divisor)
//a: the first integer
//b: the second integer
void gcd(mpz_t d, mpz_t a, mpz_t b) {
    //Initialize temp + auxilary vars to prevent inputs from changing
    mpz_t temp, changing_b, changing_a;
    mpz_inits(temp, changing_b, changing_a, NULL);
    mpz_set(changing_b, b);
    mpz_set(changing_a, a);

    while (mpz_sgn(changing_b) != 0) {
        mpz_set(temp, changing_b);
        mpz_mod(changing_b, changing_a, changing_b);
        mpz_set(changing_a, temp);
    }
    mpz_set(d, changing_a);
    mpz_clears(temp, changing_b, changing_a, NULL);
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
    while (mpz_sgn(r_prime) != 0) {
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

    mpz_clears(r, r_prime, i_prime, q, temp, NULL);
}

//Computes base^exponent mod modulus, then puts value in out
//
//out: output of the computed value
//base: number that is the base value
//exponent: number that is the exponent of the base
//modulus: number that is be used to mod value
void pow_mod(mpz_t out, mpz_t base, mpz_t exponent, mpz_t modulus) {
    //Initialize variable that holds base, exponent, out  to prevent unexpected manipulation of inputs
    mpz_t changing_base, changing_exponent, changing_out;

    mpz_init_set(changing_out, out);
    mpz_set_ui(changing_out, 1);
    mpz_init_set(changing_base, base);
    mpz_init_set(changing_exponent, exponent);

    while (mpz_sgn(changing_exponent) == 1) {
        if (mpz_odd_p(changing_exponent) != 0) {
            mpz_mul(changing_out, changing_out, changing_base);
            mpz_mod(changing_out, changing_out, modulus);
        }
        mpz_mul(changing_base, changing_base, changing_base);
        mpz_mod(changing_base, changing_base, modulus);
        mpz_fdiv_q_ui(changing_exponent, changing_exponent, 2);
    }
    mpz_set(out, changing_out);
    mpz_clears(changing_base, changing_exponent, NULL);
}
//Tests if a number is prime via the Miller-Rabin Algorithm
//
//n: the number to test if prime or not
//iters: amount of iterations to test prime number with
bool is_prime(mpz_t n, uint64_t iters) {
    //Initialize modulus checker to see if n is even (or n % two = zero)
    mpz_t mod_checker;
    mpz_init(mod_checker);
    mpz_mod_ui(mod_checker, n, 2);

    //Check if n is even
    if (mpz_sgn(mod_checker) == 0) {
        //Check if n is two
        if (mpz_cmp_ui(n, 2) == 0) {
            mpz_clear(mod_checker);
            return true;
        }
        //Else its not prime (including zero)
        mpz_clear(mod_checker);
        return false;
    }

    //Check if n is one(not prime), or three(prime) to avoid impossible calculations
    if (mpz_cmp_ui(n, 1) == 0) {
        return false;
    }
    if (mpz_cmp_ui(n, 3) == 0) {
        return true;
    }

    //Initialize s and r from n
    mpz_t s, r;
    mpz_inits(s, r, NULL);
    mpz_sub_ui(r, n, 1);

    //Divides r until r is odd and increments s, satisfying equation from pseudo
    while (mpz_even_p(r) != 0) {
        mpz_fdiv_q_ui(r, r, 2);
        mpz_add_ui(s, s, 1);
    }

    //Initialize variables that generates random a in range two to n-two
    mpz_t range, a;
    mpz_inits(range, a, NULL);
    mpz_sub_ui(range, n, 3);

    //Initialize variables used in algorithm
    mpz_t y, j, two, conditional_n, conditional_s;
    mpz_inits(y, j, two, conditional_n, conditional_s, NULL);
    mpz_sub_ui(conditional_n, n, 1);
    mpz_sub_ui(conditional_s, s, 1);
    mpz_set_ui(two, 2);

    //Follow M-R Algorithm
    for (uint64_t i = 1; i <= iters; i++) {
        mpz_urandomm(a, state, range);
        mpz_add_ui(a, a, 2);
        pow_mod(y, a, r, n);
        if ((mpz_cmp_ui(y, 1) != 0) && (mpz_cmp(y, conditional_n) != 0)) {
            mpz_set_ui(j, 1);
            while ((mpz_cmp(j, conditional_s) <= 0) && (mpz_cmp(y, conditional_n) != 0)) {
                pow_mod(y, y, two, n);

                if (mpz_cmp_ui(y, 1) == 0) {
                    mpz_clears(s, r, range, a, y, j, two, conditional_n, conditional_s, NULL);
                    return false;
                }
                mpz_add_ui(j, j, 1);
            }
            if (mpz_cmp(y, conditional_n) != 0) {
                mpz_clears(s, r, range, a, y, j, two, conditional_n, conditional_s, NULL);
                return false;
            }
        }
    }
    mpz_clears(s, r, range, a, y, j, two, conditional_n, conditional_s, NULL);
    return true;
}
//Generates a number that is bits long, then tests if prime using is_prime
//Keeps generating until both conditions are satisfied
//
//p: the stored prime number
//bits: value of how large the number must be bit wise
//iters: the amount of iterations to test if a number is prime
void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
    mpz_urandomb(p, state, bits);
    while (!is_prime(p, iters) && (mpz_sizeinbase(p, 2) < bits)) {
        mpz_urandomb(p, state, bits);
    }
}
