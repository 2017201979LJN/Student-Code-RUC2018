#include "qsort.h"

#include <string.h> // memcpy()
#include <limits.h> // CHAR_BIT
#include <stdint.h> // uint*_t

typedef uint8_t _type_8;
typedef uint16_t _type_16;
typedef uint32_t _type_32;
typedef uint64_t _type_64;

#define _QSORT_SPECIALIZED_TYPE      _type_8
#define _QSORT_SPECIALIZED_TYPE_SIZE sizeof(_type_8)
#include "qsort_specialize.h"
#undef _QSORT_SPECIALIZED_TYPE
#undef _QSORT_SPECIALIZED_TYPE_SIZE

#define _QSORT_SPECIALIZED_TYPE      _type_16
#define _QSORT_SPECIALIZED_TYPE_SIZE sizeof(_type_16)
#include "qsort_specialize.h"
#undef _QSORT_SPECIALIZED_TYPE
#undef _QSORT_SPECIALIZED_TYPE_SIZE

#define _QSORT_SPECIALIZED_TYPE      _type_32
#define _QSORT_SPECIALIZED_TYPE_SIZE sizeof(_type_32)
#include "qsort_specialize.h"
#undef _QSORT_SPECIALIZED_TYPE
#undef _QSORT_SPECIALIZED_TYPE_SIZE

#define _QSORT_SPECIALIZED_TYPE      _type_64
#define _QSORT_SPECIALIZED_TYPE_SIZE sizeof(_type_64)
#include "qsort_specialize.h"
#undef _QSORT_SPECIALIZED_TYPE
#undef _QSORT_SPECIALIZED_TYPE_SIZE

#if __SIZEOF_INT128__ == 16
typedef unsigned __int128 _type_128;
#define _QSORT_SPECIALIZED_TYPE      _type_128
#define _QSORT_SPECIALIZED_TYPE_SIZE sizeof(_type_128)
#include "qsort_specialize.h"
#undef _QSORT_SPECIALIZED_TYPE
#undef _QSORT_SPECIALIZED_TYPE_SIZE
#endif

// General implementation.
#include "qsort_specialize.h"

void _qsort(void *base,
            size_t n,
            size_t obj_size,
            compare_func_t compare_func) {
#define _try_specialized_implementation(target_size) {      \
    if (obj_size == target_size / CHAR_BIT) {               \
        (_qsort_type_##target_size)(base, n, compare_func); \
        return;                                             \
    }                                                       \
}
    _try_specialized_implementation(8);
    _try_specialized_implementation(16);
    _try_specialized_implementation(32);
    _try_specialized_implementation(64);
#if __SIZEOF_INT128__ == 16
    _try_specialized_implementation(128);
#endif

    // Fallback to general implementation.
    _qsort_general(base, n, obj_size, compare_func);
}
