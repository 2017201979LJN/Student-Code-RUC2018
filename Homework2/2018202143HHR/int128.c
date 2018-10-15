#include "int128.h"

#include <assert.h>

#include "util.h"

#if __SIZEOF_INT128__ == 16
const _uint128_t _UINT128_ZERO = 0;
const _uint128_t _UINT128_ONE = 1;
#else
const _uint128_t _UINT128_ZERO = { 0, 0 };
const _uint128_t _UINT128_ONE = { 1, 0 };
#endif

#if __SIZEOF_INT128__ != 16
_uint128_t _uint128_make(uint64_t high64, uint64_t low64) {
    _uint128_t result;
    result.high64 = high64;
    result.low64 = low64;
    return result;
}

_uint128_t _uint128_from_uint64(uint64_t x) {
    return _uint128_make(0, x);
}

uint64_t _uint128_get_high64(const _uint128_t x) {
    return x.high64;
}

uint64_t _uint128_get_low64(const _uint128_t x) {
    return x.low64;
}

void _uint128_to_two_uint64(const _uint128_t x, uint64_t *high64, uint64_t *low64) {
    *high64 = x.high64;
    *low64 = x.low64;
}

bool _uint128_to_bool(const _uint128_t x) {
    return x.high64 || x.low64;
}

int _uint128_count_left_zeros(const _uint128_t x) {
    int result = _util_count_uint64_left_zeros(x.high64);
    if (result == 64) result += _util_count_uint64_left_zeros(x.low64);
    return result;
}

int _uint128_compare(const _uint128_t x, const _uint128_t y) {
    if (x.high64 != y.high64) return x.high64 < y.high64 ? -1 : 1;
    else if (x.low64 != y.low64) return x.low64 < y.low64 ? -1 : 1;
    else return 0;
}

_uint128_t _uint128_shl(const _uint128_t x, int n) {
    assert(n >= 0);
    if (n == 0) return x;
    if (n >= 128) return _UINT128_ZERO;
    if (n >= 64) return _uint128_make(x.low64 << (n - 64), 0);
    return _uint128_make((x.high64 << n) | (x.low64 >> (64 - n)), x.low64 << n);
}

_uint128_t _uint128_shr(const _uint128_t x, int n) {
    assert(n >= 0);
    if (n == 0) return x;
    if (n >= 128) return _UINT128_ZERO;
    if (n >= 64) return _uint128_make(0, x.high64 >> (n - 64));
    return _uint128_make(x.high64 >> n, (x.low64 >> n) | (x.high64 << (64 - n)));
}

_uint128_t _uint128_and(const _uint128_t x, const _uint128_t y) {
    return _uint128_make(x.high64 & y.high64, x.low64 & y.low64);
}

_uint128_t _uint128_or(const _uint128_t x, const _uint128_t y) {
    return _uint128_make(x.high64 | y.high64, x.low64 | y.low64);
}

_uint128_t _uint128_xor(const _uint128_t x, const _uint128_t y) {
    return _uint128_make(x.high64 ^ y.high64, x.low64 ^ y.low64);
}

_uint128_t _uint128_not(const _uint128_t x) {
    return _uint128_make(~x.high64, ~x.low64);
}

_uint128_t _uint128_add(const _uint128_t x, const _uint128_t y) {
    _uint128_t result;
    result.low64 = x.low64 + y.low64;
    result.high64 = x.high64 + y.high64;

    // If the add of two low64 overflowed, the result will less than an
    // addend.
    if (result.low64 < max(x.low64, y.low64)) result.high64++;

    return result;
}

_uint128_t _uint128_sub(const _uint128_t x, const _uint128_t y) {
    _uint128_t result;
    result.low64 = x.low64 - y.low64;
    result.high64 = x.high64 - y.high64;

    // If the add of two low64 underflowed, the result will larger than the
    // subtracted.
    if (result.low64 > x.low64) result.high64--;

    return result;
}

_uint128_t _uint128_mul(const _uint128_t x, const _uint128_t y) {
    _uint128_t result = _UINT128_ZERO,
               tx = x, ty = y;
    
    // The time complexity is depending on the length of ty.
    // So choose a shorter x or y as ty.
    if (_uint128_count_left_zeros(tx) > _uint128_count_left_zeros(ty)) {
        // Swap them.
        _uint128_t t = tx;
        tx = ty;
        ty = t;
    }

    // From lowest to highest, consider each bit of ty.
    for (_uint128_t tx = x, ty = y; _uint128_to_bool(ty); ty = _uint128_shr(ty, 1)) {
        // If the i-th bit is 1, add (tx << i) to result.
        if (_uint128_to_bool(_uint128_and(ty, _UINT128_ONE))) {
            result = _uint128_add(result, tx);
        }

        // Shift a bit every cycle, to make sure that in the i-th cycle, the
        // value of tx is the original (tx << i).
        tx = _uint128_shl(tx, 1);
    }

    return result;
}

_uint128_t _uint128_div(const _uint128_t x, const _uint128_t y) {
    assert(_uint128_to_bool(y));
    
    // When x < y, x / y = 0.
    if (_uint128_compare(x, y) == -1) return _UINT128_ZERO;

    // Let t be the length of y, Get the highest t bits of x.
    int lx = 128 - _uint128_count_left_zeros(x),
        ly = 128 - _uint128_count_left_zeros(y),
        current_lowest_bit = lx - ly;
    _uint128_t tx = _uint128_shr(x, current_lowest_bit),
               ty = y;

    _uint128_t result = _UINT128_ZERO;

    // From current lowest bit to 0, each bit of x will contribute one bit to
    // the quotient.
    for (; current_lowest_bit >= 0; current_lowest_bit--) {
        result = _uint128_shl(result, 1);
        // tx >= ty
        if (_uint128_compare(tx, ty) != -1) {
            tx = _uint128_sub(tx, ty);
            result = _uint128_or(result, _UINT128_ONE);
        }

        // Add the next bit.
        if (current_lowest_bit > 0) {
            tx = _uint128_shl(tx, 1);
            // (x >> (current_lowest_bit - 1)) & 1
            if (_uint128_to_bool(_uint128_and(_uint128_shr(x, current_lowest_bit - 1),
                                              _UINT128_ONE))) {
                tx = _uint128_or(tx, _UINT128_ONE);
            }
        }
    }

    return result;
}
#endif
