#include "bignum.h"

#include <assert.h>
#include <string.h>
#include <stdbool.h>

void _bignum_init(_bignum_t *x, uint64_t value) {
    assert(value <= _BIGNUM_BLOCK_MODULAR);

    memset(x, 0, sizeof(_bignum_t));
    x->n = 1;
    x->blocks[0] = value;
}

void _bignum_add_bignum(_bignum_t *a, const _bignum_t *b) {
    assert(a->n <= _BIGNUM_SIZE);
    assert(b->n <= _BIGNUM_SIZE);

    if (a->n < b->n) a->n = b->n;
    
    register bool carry = false;
    for (register int i = 0; i < a->n; i++) {
        register uint64_t t = a->blocks[i] + b->blocks[i];
        if (carry) {
            t++;
            carry = false;
        }
        
        // Overflowed?
        if (t >= _BIGNUM_BLOCK_MODULAR) {
            t -= _BIGNUM_BLOCK_MODULAR;
            carry = true;
        }

        a->blocks[i] = t;
    }

    if (carry) {
        a->blocks[a->n] = 1;
        a->n++;
    }

    assert(a->n <= _BIGNUM_SIZE);
}

void _bignum_add_uint64(_bignum_t *a, uint64_t b) {
    assert(a->n <= _BIGNUM_SIZE);

    // Carry is not implemented.
    assert(a->blocks[0] + b < _BIGNUM_BLOCK_MODULAR);
    
    a->blocks[0] += b;
}

void _bignum_mul_uint64(_bignum_t *a, uint64_t b) {
    assert(a->n <= _BIGNUM_SIZE);

    // For b <= 10, a block won't overflow.
    assert(b <= 10);

    _bignum_t result;
    _bignum_init(&result, 0);

    int carry = 0;
    for (int i = 0; i < a->n; i++) {
        uint64_t tmp = a->blocks[i] * b + carry;

        if (tmp >= _BIGNUM_BLOCK_MODULAR) {
            carry = tmp / _BIGNUM_BLOCK_MODULAR;
            tmp %= _BIGNUM_BLOCK_MODULAR;
        } else carry = 0;

        a->blocks[i] = tmp;
    }

    if (carry) {
        a->blocks[a->n] = carry;
        a->n++;
    }
    
    assert(a->n <= _BIGNUM_SIZE);
}

void _bignum_output_decimal(const _bignum_t *x, char *buffer) {
    register char *p = buffer;
    for (int i = 0; i < x->n; i++) {
        register uint64_t t = x->blocks[i];

        // Convert a block to decimal.
        for (register int j = 0; j < _BIGNUM_BLOCK_LENGTH; j++) {
            *p++ = t % 10 + '0';
            t /= 10;
        }
    }

    // Remove the leading zeros (in the end of the string currently).
    do *p-- = '\0'; while (p != buffer && *p == '0');

    // Reverse the string.
    for (register char *s = buffer, *t = p; s < t; s++, t--) {
        register char tmp = *t;
        *t = *s;
        *s = tmp;
    }
}
