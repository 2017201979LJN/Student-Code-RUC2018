#include "int256.h"

const _uint256_t _UINT256_ZERO = { 0, 0 };
const _uint256_t _UINT256_ONE = { 1, 0 };

_uint256_t _uint256_make(_uint128_t high128, _uint128_t low128) {
    _uint256_t result;
    result.high128 = high128;
    result.low128 = low128;
    return result;
}

_uint256_t _uint256_shl(const _uint256_t x, int n) {
    if (n == 0) return x;
    if (n >= 256) return _UINT256_ZERO;
    if (n >= 128) return _uint256_make(x.low128 << (n - 128), 0);
    return _uint256_make((x.high128 << n) | (x.low128 >> (128 - n)), x.low128 << n);
}

_uint256_t _uint256_and(const _uint256_t x, const _uint256_t y) {
    return _uint256_make(x.high128 & y.high128, x.low128 & y.low128);
}

_uint256_t _uint256_or(const _uint256_t x, const _uint256_t y) {
    return _uint256_make(x.high128 | y.high128, x.low128 | y.low128);
}

bool _uint256_to_bool(const _uint256_t x) {
    return x.high128 || x.low128;
}
