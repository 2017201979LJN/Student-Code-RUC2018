#ifndef _MENCI_BINARY_TREE_ZIP_HUFFMAN_HUFFMAN_H
#define _MENCI_BINARY_TREE_ZIP_HUFFMAN_HUFFMAN_H

#include <stdint.h>

#include "../bitset/bitstream.h"

void huffman_encode(void *data,
                    size_t data_length,
                    void **buffer,
                    size_t *encoded_length);
void huffman_decode(void *data,
                    void **buffer,
                    size_t *decoded_length);

#endif // _MENCI_BINARY_TREE_ZIP_HUFFMAN_HUFFMAN_H
