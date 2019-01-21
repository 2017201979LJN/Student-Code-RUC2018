#include "bitset.h"

#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

bitset_t bitset_create(size_t n) {
    size_t count = n / CHAR_BIT + !!(n % CHAR_BIT);
    bitset_t bitset = malloc(sizeof(size_t) + count);
    bitset->n = n;
    memset(bitset->buffer, 0, count);
    return bitset;
}

void bitset_destroy(bitset_t bitset) {
    free(bitset);
}

size_t bitset_sizeof(bitset_t bitset) {
    return sizeof(size_t) + bitset_buffer_size(bitset);
}

size_t bitset_buffer_size(bitset_t bitset) {
    return bitset->n / CHAR_BIT + !!(bitset->n % CHAR_BIT);
}

void *bitset_get_buffer(bitset_t bitset) {
    return bitset->buffer;
}

#define _bitset_index(pos)  (pos / CHAR_BIT)
#define _bitset_offset(pos) (pos % CHAR_BIT)

// #include <stdio.h>

void bitset_set(bitset_t bitset, size_t pos, bool value) {
    assert(pos >= 0 && pos < bitset->n);

    // fprintf(stderr, "n = %d, bitset_set(%d, %d)\n", (int)bitset->n, (int)pos, (int)value);
    
    if (value) {
        bitset->buffer[_bitset_index(pos)] |= 1U << _bitset_offset(pos);
    } else {
        bitset->buffer[_bitset_index(pos)] &= ~(1U << _bitset_offset(pos));
    }
}

bool bitset_get(bitset_t bitset, size_t pos) {
    assert(pos >= 0 && pos < bitset->n);

    return (bitset->buffer[_bitset_index(pos)] & (1U << _bitset_offset(pos))) != 0;
}
