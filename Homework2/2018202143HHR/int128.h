#ifndef _MENCI_INT128_H
#define _MENCI_INT128_H

#include <stdint.h>
#include <stdbool.h>

#if __SIZEOF_INT128__ == 16
// GCC have it's extended integer type -- __int128, which directly use x86_64
// instruction to calculate rapidly, so use it if it's available.
typedef unsigned __int128 _uint128_t;
#else
typedef struct {
    uint64_t low64, high64;
} _uint128_t;
#endif

extern const _uint128_t _UINT128_ZERO;
extern const _uint128_t _UINT128_ONE;

int _uint128_count_left_zeros(const _uint128_t x);

#if __SIZEOF_INT128__ == 16
#define _uint128_make(high64, low64)             ((((_uint128_t)(high64)) << 64) | (low64))
#define _uint128_from_uint64(x)                  ((_uint128_t)(x))
#define _uint128_get_high64(x)                   ((x) >> 64)
#define _uint128_get_low64(x)                    ((x) << 64 >> 64)
#define _uint128_to_two_uint64(x, high64, low64) ((*(high64)) = ((x) >> 64), \
                                                  (*(low64)) = ((x) << 64 >> 64)))
#define _uint128_to_bool(x)                      ((bool)(x))
#define _uint128_count_left_zeros(x)             ((_util_count_uint64_left_zeros(x >> 64) != 64) \
                                                  ? _util_count_uint64_left_zeros(x >> 64)       \
                                                  : (64 + _util_count_uint64_left_zeros(x << 64 >> 64)))
#define _uint128_compare(x, y)                   ((x) == (y) ? 0 : ((x) < (y) ? -1 : 1))
#define _uint128_shl(x, n)                       ((n) >= 128 ? (_uint128_t)0 : ((x) << (n)))
#define _uint128_shr(x, n)                       ((n) >= 128 ? (_uint128_t)0 : ((x) >> (n)))
#define _uint128_and(x, y)                       ((x) & (y))
#define _uint128_or(x, y)                        ((x) | (y))
#define _uint128_xor(x, y)                       ((x) ^ (y))
#define _uint128_not(x)                          (~(x))
#define _uint128_add(x, y)                       ((x) + (y))
#define _uint128_sub(x, y)                       ((x) - (y))
#define _uint128_mul(x, y)                       ((x) * (y))
#define _uint128_div(x, y)                       ((x) / (y))
#else
_uint128_t _uint128_make(uint64_t high64, uint64_t low64);
_uint128_t _uint128_from_uint64(uint64_t x);

uint64_t   _uint128_get_high64(const _uint128_t x);
uint64_t   _uint128_get_low64(const _uint128_t x);
void       _uint128_to_two_uint64(const _uint128_t x, uint64_t *high64, uint64_t *low64);
bool       _uint128_to_bool(const _uint128_t x);

int        _uint128_compare(const _uint128_t x, const _uint128_t y);

_uint128_t _uint128_shl(const _uint128_t x, int n);
_uint128_t _uint128_shr(const _uint128_t x, int n);
_uint128_t _uint128_and(const _uint128_t x, const _uint128_t y);
_uint128_t _uint128_or(const _uint128_t x, const _uint128_t y);
_uint128_t _uint128_xor(const _uint128_t x, const _uint128_t y);
_uint128_t _uint128_not(const _uint128_t x);

_uint128_t _uint128_add(const _uint128_t x, const _uint128_t y);
_uint128_t _uint128_sub(const _uint128_t x, const _uint128_t y);
_uint128_t _uint128_mul(const _uint128_t x, const _uint128_t y);
_uint128_t _uint128_div(const _uint128_t x, const _uint128_t y);
#endif

#endif // _MENCI_INT128_H
