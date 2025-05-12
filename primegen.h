#ifndef PRIMEGEN_H
#define PRIMEGEN_H

#include <stdint.h>
#include <stdlib.h>

typedef uint64_t prime; // Type for storing prime numbers

// Generates the number of primes specified by list_size. Returns a pointer to
// an array that is the list of every prime number generated (heap-allocated).
prime* generate_primes(size_t list_size);

#endif
