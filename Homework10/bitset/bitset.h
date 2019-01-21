#ifndef _MENCI_BINARY_TREE_ZIP_BITSET_BITSET_H
#define _MENCI_BINARY_TREE_ZIP_BITSET_BITSET_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
    size_t n;
    char buffer[0];
} *bitset_t;

bitset_t bitset_create(size_t n);
void bitset_destroy(bitset_t bitset);
size_t bitset_sizeof(bitset_t bitset);
size_t bitset_buffer_size(bitset_t bitset);
void *bitset_get_buffer(bitset_t bitset);
void bitset_set(bitset_t bitset, size_t pos, bool value);
bool bitset_get(bitset_t bitset, size_t pos);

void *bitset_serialize(bitset_t bitset);
bitset_t bitset_deserialize(void *buffer);

#endif // _MENCI_BINARY_TREE_ZIP_BITSET_BITSET_H
