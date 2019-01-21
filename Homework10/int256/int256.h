#ifndef _MENCI_BINARY_TREE_ZIP_AVL_TREE_INT256_INT256_H
#define _MENCI_BINARY_TREE_ZIP_AVL_TREE_INT256_INT256_H

#include <stdbool.h>

typedef unsigned __int128 _uint128_t;

typedef struct {
    _uint128_t low128, high128;
} _uint256_t;

const _uint256_t _UINT256_ZERO;
const _uint256_t _UINT256_ONE;

_uint256_t _uint256_shl(const _uint256_t x, int n);
_uint256_t _uint256_and(const _uint256_t x, const _uint256_t y);
_uint256_t _uint256_or(const _uint256_t x, const _uint256_t y);
bool _uint256_to_bool(const _uint256_t x);

#endif // _MENCI_BINARY_TREE_ZIP_AVL_TREE_INT256_INT256_H
