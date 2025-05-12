#include <stdlib.h>

#include "bmap.h"
#include "primegen.h"

// Converts a number into its position in a 2-case map.
static size_t num_to_pos(size_t num);

// Converts a 2-case position into the associated number.
static size_t pos_to_num(size_t pos);

// Marks the multiples of a prime number through the assoiated bitmap as
// non-primes starting from a specific index.
static void progagate_prime(struct bmap bmap[], prime prime_num,
                            size_t start_index);

prime* generate_primes(size_t list_size) {
    // Create the list of prime numbers
    prime* prime_list = malloc(list_size * sizeof(prime));
    prime_list[0] = 2; // 2 is prime
    size_t list_index = 1;

    // Create and initialize bitmap
    const size_t bmap_increment = list_size; // Size of bitmap allocations
    struct bmap* prime_map = bmap_create(bmap_increment, true);
    bmap_set(prime_map, num_to_pos(1), false); // 1 is not prime
    size_t bmap_index = 1;

    // Find prime numbers until list is filled
    for (; list_index < list_size; list_index++, bmap_index++) {
        // Find the next prime number
        bmap_index = bmap_find_next(prime_map, bmap_index, true, RIGHT);

        // If end of bitmap reached, need to extend it
        if (bmap_index == prime_map->length) {
            // Extend bitmap
            bmap_extend(prime_map, bmap_increment, true);

            // Update new bitmap entries with previous primes
            for (size_t i = 1; i < list_index; i++)
                progagate_prime(prime_map, prime_list[i], bmap_index);

            // Recalculate bitmap index of the next new prime
            bmap_index = bmap_find_next(prime_map, bmap_index, true, RIGHT);
        }

        // Update bitmap and prime list
        prime_list[list_index] = pos_to_num(bmap_index);
        progagate_prime(prime_map, pos_to_num(bmap_index), bmap_index);
    }

    bmap_free(prime_map);
    return prime_list;
}

static size_t num_to_pos(size_t num) {
    return num / 2;
}

static size_t pos_to_num(size_t pos) {
    return pos * 2 + 1;
}

static void progagate_prime(struct bmap bmap[], prime prime_num,
                            size_t start_index) {
    size_t multiple =
        (pos_to_num(start_index) + prime_num - 1) / prime_num * prime_num;

    // Don't mark prime number as non-prime
    if (multiple == prime_num)
        multiple += 2 * prime_num;

    // Multiples of 2 aren't on the bitmap
    if (multiple % 2 == 0)
        multiple += prime_num;

    while (num_to_pos(multiple) < bmap->length) {
        bmap_set(bmap, num_to_pos(multiple), false);
        multiple += 2 * prime_num;
    }
}
