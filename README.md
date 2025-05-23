This is a basic prime number generator based on the Sieve of Eratosthenes that I'm trying to make as fast as possible in C. The project comes with its own custom-made bitmap library, and uses a 2-case bit pattern to reduce memory footprint.

# Usage Guide

## How to build

### Build Tools Required

-   A C compiler (GCC or Clang)
-   GNU Make

### Build Steps

-   In the Makefile, change the value of `CC` to whichever compiler you want to use.
-   `make release`

## How to use

`./primegen [flags]` (by default, the program will generate the first 1 million prime numbers)

### Command line arguments

-   `--num [x]` or `-n [x]`: the number of prime numbers to generate.
-   `--write` or`-w`: will write all prime numbers generated to a `primes.txt` file.
