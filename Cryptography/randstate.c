#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <gmp.h>

gmp_randstate_t state;

//randstate initialization function: initializes globlal random state with Mersenne Twister algorithm
void randstate_init(uint64_t seed) {
    srandom(seed); //fetches a random seed
    gmp_randinit_mt(state); //initializing the state
    gmp_randseed_ui(state, seed);
}

//randstate clear function: clears and frees all memory used
void randstate_clear(void) {
    gmp_randclear(state); //clears the state
}
