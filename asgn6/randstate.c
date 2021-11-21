#include "randstate.h"
#include <gmp.h>

//Initializes the random number generation using state and seed
//
//seed: specifies seed to be used for the random number generation
gmp_randstate_t state;
void randstate_init(uint64_t seed) {
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, seed);
}

//Clears the memory used by the state
void randstate_clear(void) {
    gmp_randclear(state);
}
