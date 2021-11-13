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
	while(mpz_sgn(b) != 0) {
		mpz_set(temp, b);
		mpz_mod(b, a, b);
		mpz_set(a, temp);	
	}

	mpz_clear(&temp);
}

//Finds the modular multiplicative inverse of n using a and stores in i
//
//i: the modular inverse of a
//a: the number
//n: the modulus number
//
//DESCRIPTION NEEDS WORK
void mod_inverse(mpz_t i, mpz_t a, mpz_t n) {
	mpz_t r;
	mpz_t r_prime;
	mpz_init_set(r, n);
	mpz_init_set(r_prime, a);

	mpz_t i;
	mpz_t i_prime;
	mpz_init_set_ui(i, 0);
	mpz_init_set_ui(i_prime, 1);
	
	mpz_t q;
	mpz_init(q);
	mpz_t temp;
	mpz_init(temp);
	while(mpz_sgn(r) != 0) {
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

	if(mpz_cmp_ui(r, 1) > 0) {
		mpz_set_ui(i, 0);
	}

	if(mpz_sgn(i) == -1) {
		mpz_add(i, i, n);
	}

	mpz_clear(&r);
	mpz_clear(&r_prime);
	mpz_clear(&i_prime);
	mpz_clear(&q);
	mpz_clear(&temp);
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
	mpz_clear(&changing_base);
	mpz_clear(&odd_checker);
}
//Tests if a number is prime via the Miller-Rabin Algorithm
//
//n: the number to test if prime or not
//iters: amount of iterations to test prime number with
bool is_prime(mpz_t n, uint64_t iters) {
	mpz_t s;
	mpz_init(s);
	mpz_t r;
	mpz_init(r);

	//Initialize variables that generates random a in range two to n-two
	mpz_t range;
	mpz_init(range);
	mpz_sub_ui(range, n, 3);
	mpz_t a;
        mpz_init(a);
	
	//Initialize variables used in algorithm
	mpz_t y;
	mpz_init(y);
	mpz_t j;
	mpz_init(j);
	for(uint64_t i = 0; i <= iters; i++) {
		mpz_urandomm(a, state, range);
		pow_mod(y,a,r,n);
		if(mpz_cmp_ui(y, 1) != 0) &&  {
		
		}
		
	}
}
//Generates a number that is bits long, then tests if prime using is_prime
//
//p: the stored prime number
//bits: value of how large the number must be bit wise
//iters: the amount of iterations to test if a number is prime
void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
	//Make sure p isn't prime to generate new one
	mpz_set_ui(p, 4);

	mp_bitcnt_t bits;
	while(!is_prime(p, iters)) {
		mpz_rrandomb(p, state, bits);
		if(is_prime(p, iters)) {
			break;
		}
}}
