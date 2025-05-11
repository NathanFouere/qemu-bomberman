#ifndef LCG_RANDOM_H
#define LCG_RANDOM_H

// Linear Congruential Generator (LCG) pseudo-random number source
// Constants based on "Numerical Recipes"
//   modulus m = 2^31, multiplier a = 1103515245, increment c = 12345

static unsigned int lcg_seed = 1;

// Sets the seed for the LCG. Call this once before generating random numbers.
// static inline void srand_lcg(unsigned int seed) {
//     lcg_seed = seed & 0x7FFFFFFF;
// }

// Returns a 15-bit pseudo-random number in the range [0, 32767].
// Advances the internal seed using a standard LCG step.
static inline unsigned int pseudoRand(void) {
    lcg_seed = (1103515245U * lcg_seed + 12345U) & 0x7FFFFFFF;
    return (lcg_seed >> 16) & 0x7FFF;
}

#endif // LCG_RANDOM_H