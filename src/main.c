#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/main.h"
#include "../include/primegen.h"

#define DEFAULT_LIST_SIZE 1000000 // Default size of prime number list

// Settings affected by command line arguments
size_t list_size = DEFAULT_LIST_SIZE; // Size of prime number list
bool write_values = false; // Whether to write the primes to a file or not

int main(int argc, char* argv[]) {
    // Parse and interpret command line arguments
    parse_arguments(argc, argv);

    // Generate prime numbers
    prime* prime_list = generate_primes(list_size);

    // Print last prime number in the list
    printf("%zuth prime number: %zu\n", list_size, prime_list[list_size - 1]);

    // Write all prime numbers to a file if specified
    if (write_values)
        write_primes(prime_list);

    free(prime_list);
    return 0;
}

void parse_arguments(int argc, char* argv[]) {
    // Check if there are arguments
    if (argc < 2)
        return;

    // Parse individual arguments
    for (size_t i = 1; i < (size_t) argc; i++) {
        // Check for "--num" and "-n" (number of prime numbers to be generated)
        if (strcmp(argv[i], "--num") == 0 || strcmp(argv[i], "-n") == 0) {
            // Check for missing value after argument
            if (i + 1 >= (size_t) argc) {
                printf("Missing value after argument \"--num\".\n");
                exit(-1);
            }

            // Check that the value fed is a valid positive integer
            char* arg_num = argv[i + 1];
            for (size_t j = 0; j < strlen(arg_num); j++) {
                if (arg_num[j] < '0' || arg_num[j] > '9') {
                    printf("Invalid value for argument \"--num\". Value must "
                           "be a valid positive integer.\n");
                    exit(-1);
                }
            }

            // Check that the value fed is not 0
            size_t num;
            sscanf(arg_num, "%zu", &num);
            if (num == 0) {
                printf("Invalid value for argument \"--num\". Value must "
                       "be bigger than 0.\n");
                exit(-1);
            }

            // Edit the prime list size with the value fed
            sscanf(arg_num, "%zu", &list_size);
            i++; // Increment i to jump 2 tokens in the argument list
        }

        // Check for "--write" and "-w" (writing prime numbers to a file)
        else if (strcmp(argv[i], "--write") == 0
                 || strcmp(argv[i], "-w") == 0) {
            write_values = true;
        }

        // Unrecognized argument
        else {
            printf("Unrecognized argument: \"%s\"\n", argv[i]);
            exit(-1);
        }
    }
}

void write_primes(prime prime_list[]) {
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
