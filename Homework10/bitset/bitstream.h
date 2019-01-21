#ifndef _MENCI_BINARY_TREE_ZIP_BITSET_BITSTREAM_H
#define _MENCI_BINARY_TREE_ZIP_BITSET_BITSTREAM_H

#include "bitset.h"

#include <stdbool.h>

typedef struct {
    bitset_t bitset;
    size_t pos;
} *bitstream_t;

bitstream_t bitstream_create(bitset_t bitset, size_t pos);
void bitstream_destroy(bitstream_t bitstream);
void bitstream_write(bitstream_t bitstream, bool value);
void bitstream_write_data(bitstream_t bitstream, void *data, size_t length);
bool bitstream_read(bitstream_t bitstream);
void bitstream_read_data(bitstream_t bitstream, void *result, size_t length);
void bitstream_close(bitstream_t bitstream);

#endif // _MENCI_BINARY_TREE_ZIP_BITSET_BITSTREAM_H
