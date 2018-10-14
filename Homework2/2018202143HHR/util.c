#include "util.h"

uint32_t _util_uint64_high32(uint64_t x) {
    return x >> 32;
}

uint32_t _util_uint64_low32(uint64_t x) {
    return x << 32 >> 32;
}

#ifndef __GNUC__
int _util_count_uint64_left_zeros(register uint64_t x) {
    register int res = 64;
    while (x) {
        res--;
        x >>= 1;
    }
    return res;
}
#endif
