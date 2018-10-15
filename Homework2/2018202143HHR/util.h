#ifndef _MENCI_UTIL_H
#define _MENCI_UTIL_H

#include <stdint.h>

#define max(a, b) ((a) < (b) ? (b) : (a))
#define min(a, b) ((a) < (b) ? (a) : (b))

#define _util_uint64_shr_safe(x, n) ((n) > 64 ? 0ull : ((x) >> (n)))

uint32_t _util_uint64_high32(uint64_t x);
uint32_t _util_uint64_low32(uint64_t x);

#ifdef __GNUC__
#define _util_count_uint64_left_zeros(x) ((x) ? __builtin_clzll(x) : 64)
#else
int      _util_count_uint64_left_zeros(uint64_t x);
#endif

#define INT3 asm ("int3")

#endif // _MENCI_UTIL_H
