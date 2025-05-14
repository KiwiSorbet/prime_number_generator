#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/bmap.h"

struct bmap* bmap_create(size_t length_bits, bool init) {
    // Allocate memory for bitmap struct
    struct bmap* bmap = malloc(sizeof(struct bmap));
    if (bmap == NULL)
        return NULL;

    // Get the length in bits to allocate (rounded up to the nearest chunk)
    bmap->length = ((length_bits + CHUNK_SIZE_BITS - 1) / CHUNK_SIZE_BITS
                    * CHUNK_SIZE_BITS);

    // Allocate memory for bitmap
    bmap->map = calloc(bmap->length / CHUNK_SIZE_BITS, CHUNK_SIZE);
    if (bmap->map == NULL) {
        printf("Bitmap allocation failed\n");
        exit(-1);
    }

    // Set all bits to 1 if needed
    if (init == true)
        memset(bmap->map, 0b11111111, bmap->length / 8);

    return bmap;
}

void bmap_extend(struct bmap* bmap, size_t extend_length, bool init) {
    // Create the new larger bitmap
    size_t new_length = (bmap->length + extend_length + CHUNK_SIZE_BITS - 1)
                      / CHUNK_SIZE_BITS * CHUNK_SIZE_BITS;
    bmap->map = realloc(bmap->map, new_length / 8);
    if (bmap->map == NULL) {
        printf("Bitmap allocation failed\n");
        exit(-1);
    }

    // Update bitmap length
    size_t old_length = bmap->length;
    bmap->length = new_length;

    // Set all newly allocated bits to chose
    memset(bmap->map + old_length / CHUNK_SIZE_BITS,
           (init) ? 0b11111111 : 0b00000000, (new_length - old_length) / 8);
}

void bmap_free(struct bmap* bmap) {
    free(bmap->map);
    free(bmap);
}

bool bmap_get(const struct bmap* bmap, size_t index) {
    // Index out of bounds always returns false
    if (index >= bmap->length)
        return false;

    // Get the index of the corresponding chunk and the bit's offset
    size_t chunk_index = index / CHUNK_SIZE_BITS;
    size_t offset = index % CHUNK_SIZE_BITS;

    // Extract the relevant bit from the map
    bmap_chunk binary_control =
        (bmap_chunk) pow(2, CHUNK_SIZE_BITS - 1) >> offset;
    binary_control &= bmap->map[chunk_index];

    return (binary_control == 0) ? false : true;
}

size_t bmap_find_next(const struct bmap* bmap, size_t index, bool value,
                      enum direction dir) {
    size_t cursor = index;

    // Walk right
    if (dir == RIGHT)
        while (cursor < bmap->length && bmap_get(bmap, cursor) != value)
            cursor++;
    // Walk left
    else if (dir == LEFT)
        while (bmap_get(bmap, cursor) != value)
            cursor--;

    // Didn't find any occurence
    if (cursor >= bmap->length)
        return bmap->length;

    return cursor;
}

void bmap_set(struct bmap* bmap, size_t index, bool value) {
    // Index out of bounds
    if (index >= bmap->length)
        return;

    // Get the index of the corresponding chunk and the bit's offset
    size_t chunk_index = index / CHUNK_SIZE_BITS;
    size_t offset = index % CHUNK_SIZE_BITS;

    // Set to true
    if (value == true) {
        bmap_chunk binary_control =
            (bmap_chunk) pow(2, CHUNK_SIZE_BITS - 1) >> offset;
        bmap->map[chunk_index] |= binary_control;
    }
    // Set to false
    else {
        bmap_chunk binary_control =
            ~((bmap_chunk) pow(2, CHUNK_SIZE_BITS - 1) >> offset);
        bmap->map[chunk_index] &= binary_control;
    }
}

void bmap_set_mul(struct bmap* bmap, size_t index, size_t cnt, bool value) {
    for (size_t i = index; i < index + cnt; i++)
        bmap_set(bmap, i, value);
}

void bmap_print(const struct bmap* bmap) {
    size_t byte_length = (bmap->length + 7) / 8;

    printf("------------------------\n");

    // Print each byte
    for (size_t i = 0; i < byte_length; i++) {
        printf("%6zu | ", i * 8);

        // Print each bit in the byte
        for (size_t j = 0; j < 8; j++)
            printf("%d ", bmap_get(bmap, i * 8 + j));

        printf("\n");
    }

    printf("------------------------\n");
}
