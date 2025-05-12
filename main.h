#ifndef MAIN_H
#define MAIN_H

#include "primegen.h"

// Parses and interprets command line arguments. Edits global variables
// accordingly before prime number generation starts.
void parse_arguments(int argc, char* argv[]);

// Writes the prime numbers in the list to a primes.txt file.
void write_primes(prime prime_list[]);

#endif
