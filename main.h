#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>
#include <stdlib.h>

typedef uint64_t prime; // Type for storing prime numbers

// Converts a number into its position in a 2-case map.
size_t num_to_pos(size_t num);

// Converts a 2-case position into the associated number.
size_t pos_to_num(size_t pos);

// Marks the multiples of a prime number through the assoiated bitmap as
// non-primes starting from a specific index.
void progagate_prime(struct bmap bmap[], prime prime_num, size_t start_index);

// Parses and interprets command line arguments. Edits global variables
// accordingly before prime number generation starts.
void parse_arguments(int argc, char* argv[]);

#endif
