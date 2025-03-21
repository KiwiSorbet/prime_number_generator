#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmap.h"
#include "main.h"

#define DEFAULT_LIST_SIZE 1000000 // Default size of prime number list

// Settings affected by parser
bool write_values = false; // Whether to write the primes to a file or not
size_t list_size = DEFAULT_LIST_SIZE; // Size of prime number list

int main(int argc, char* argv[]) {
    // Parse and interpret command line arguments
    parse_arguments(argc, argv);

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

    // Print last prime number in the list
    printf("%zuth prime number: %zu\n", list_index, prime_list[list_size - 1]);

    // Write all prime numbers to a file
    if (write_values) {
        // Create file
        FILE* file = fopen("primes.txt", "w");
        if (file == NULL) {
            perror("Error opening file");
            exit(-1);
        }

        // Write values to file
        fprintf(file, "POSITION\t\tVALUE\n");
        for (size_t i = 0; i < list_size; i++)
            fprintf(file, "%zu\t\t\t\t%zu\n", i + 1, prime_list[i]);

        fclose(file);
    }

    // Deallocate memory before exiting
    free(prime_list);
    bmap_free(prime_map);
    return 0;
}

size_t num_to_pos(size_t num) {
    return num / 2;
}

size_t pos_to_num(size_t pos) {
    return pos * 2 + 1;
}

void progagate_prime(struct bmap bmap[], prime prime_num, size_t start_index) {
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

void parse_arguments(int argc, char* argv[]) {
    // Check if there are arguments
    if (argc < 2)
        return;

    // Parse individual arguments
    for (size_t i = 1; i < (size_t) argc; i++) {
        // Check for --write (writing prime numbers to a file)
        if (strcmp(argv[i], "--write") == 0) {
            write_values = true;
        }
        // Check for --num (number of prime numbers to be generated)
        else if (strcmp(argv[i], "--num") == 0) {
            // Check for missing value after argument
            if (i + 1 >= (size_t) argc) {
                printf("Missing value after argument \"--num\".\n");
                exit(-1);
            }

            // Check that the value fed is a positive integer
            bool value_is_valid = true;
            for (size_t j = 0; j < strlen(argv[i + 1]); j++) {
                if (argv[i + 1][j] < '0' || argv[i + 1][j] > '9') {
                    printf("Invalid value for argument \"--num\". Value must "
                           "be a positive integer.\n");
                    value_is_valid = false;
                    exit(-1);
                }
            }

            // Edit the prime list size with the value fed
            if (value_is_valid)
                sscanf(argv[i + 1], "%zu", &list_size);
            i++; // Increment i to jump 2 tokens in the argument list
        }
        else {
            printf("Unrecognized argument: \"%s\"\n", argv[i]);
            exit(-1);
        }
    }
}
