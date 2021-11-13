#include "randstate.h"
#include <gmp.h>

//Initializes the random generation using state and seed
//
//seed: specifies the seed for the random generation
void randstate_init(uint64_t seed) {
	gmp_randstate_t state;
	gmp_randinit_set(state);
	gmp_randseed_ui(seed);
}

//Clears the memory used by the state
//
//
void randstate_clear(void) {
	gmp_randclear(state);
}
