#ifndef _MENCI_BIGNUM_H
#define _MENCI_BIGNUM_H

#include <stdint.h>
#include "defines.h"

// The largest number to be processed by out bignum is the number after point of
// \sum\limits_{i=1}^_FP_BIGNUM_LENGTH 2^{-i}
// To calculate the number with right length, we add a '1' on the left, so the number is
// \sum\limits_{i=1}^_FP_BIGNUM_LENGTH 5^i*10^{_FP_BIGNUM_LENGTH-i}
// With python, we can calculate the number above and get its length.
#define _BIGNUM_LENGTH (_FP_BIGNUM_LENGTH + 1)

// We use uint64_t to store the value.
// Assuming that in bignum a * uint64 b, b is always <= 10, so the block modular
// can be vary large.
#define _BIGNUM_BLOCK_MODULAR 1000000000000000000ULL
#define _BIGNUM_BLOCK_LENGTH  18

// Ceil.
#define _BIGNUM_SIZE (_BIGNUM_LENGTH / _BIGNUM_BLOCK_LENGTH + \
                     !!(_BIGNUM_LENGTH % _BIGNUM_BLOCK_LENGTH))

#define _BIGNUM_OUTPUT_BUFFER_SIZE (_BIGNUM_SIZE * _BIGNUM_BLOCK_LENGTH + 1)

typedef struct {
    // Let b_i = blocks[i], its value is \sum\limits_{i=0}^{n-1}b_i 10^{18i}
    uint64_t blocks[_BIGNUM_SIZE];

    // There're n blocks of this integer.
    int n;
} _bignum_t;

void _bignum_init(_bignum_t *x, uint64_t value);
void _bignum_add_bignum(_bignum_t *a, const _bignum_t *b);
void _bignum_add_uint64(_bignum_t *a, uint64_t b);
void _bignum_mul_uint64(_bignum_t *a, uint64_t b);
void _bignum_output_decimal(const _bignum_t *x, char *buffer);

#endif // _MENCI_BIGNUM_H
