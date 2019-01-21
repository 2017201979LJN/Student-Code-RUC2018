#ifndef _MENCI_BINARY_TREE_ZIP_AVL_TREE_LZ77_LZ77_H
#define _MENCI_BINARY_TREE_ZIP_AVL_TREE_LZ77_LZ77_H

#include <stdio.h>
#include <stdint.h>
#include <limits.h>

#define LZ77_WINDOW_SIZE (1 << 16)
#define LZ77_MAX_LENGTH  ((1 << 16) - (1 << 8))
#define LZ77_MIN_LENGTH  3

void lz77_compress(void *data, size_t size, void **buffer, size_t *result_size);
void lz77_decompress(void *data, size_t size, void **buffer, size_t *result_size);

#endif // _MENCI_BINARY_TREE_ZIP_AVL_TREE_LZ77_LZ77_H
