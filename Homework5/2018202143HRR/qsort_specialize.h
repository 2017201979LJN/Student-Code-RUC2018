// This file is used for qsort()'s implementation, whose 'memcpy' is specialized
// for 1, 2, 4, 8(, 16) bytes types for better performance.

#ifdef _QSORT_SPECIALIZED_TYPE
// If _QSORT_SPECIALIZED_TYPE is defined. It means this file should represent
// a specialized implementation.
#define _specialized_memcpy(dest, src, n) \
    *(_QSORT_SPECIALIZED_TYPE *)(dest) = *(const _QSORT_SPECIALIZED_TYPE *)(src)
#define _QSORT_FUNCTION_SUFFIX _QSORT_SPECIALIZED_TYPE
#else
// Otherwise it's a general implatation, so use memcpy.
#define _specialized_memcpy memcpy
#define _QSORT_FUNCTION_SUFFIX _general
#endif

// To prevent function name conflict, redirect the function name.
#ifndef _concat
#define _concat(a, b) _concat_second_level(a, b)
#define _concat_second_level(a, b) a ## b
#endif

#define _swap                    _concat(_swap,                    _QSORT_FUNCTION_SUFFIX)
#define _insertion_sort          _concat(_insertion_sort,          _QSORT_FUNCTION_SUFFIX)
#define _heap_sort_heapify       _concat(_heap_sort_heapify,       _QSORT_FUNCTION_SUFFIX)
#define _heap_sort               _concat(_heap_sort,               _QSORT_FUNCTION_SUFFIX)
#define _quick_sort_choose_pivot _concat(_quick_sort_choose_pivot, _QSORT_FUNCTION_SUFFIX)
#define _quick_sort_partition    _concat(_quick_sort_partition,    _QSORT_FUNCTION_SUFFIX)
#define _quick_sort              _concat(_quick_sort,              _QSORT_FUNCTION_SUFFIX)
#define _qsort                   _concat(_qsort,                   _QSORT_FUNCTION_SUFFIX)

// Now include the implementation, code will be transform to expected form.
#include "qsort_implementation.h"

#undef _swap
#undef _insertion_sort
#undef _heap_sort_heapify
#undef _heap_sort
#undef _quick_sort_choose_pivot
#undef _quick_sort_partition
#undef _quick_sort
#undef _qsort

#undef _specialized_memcpy
#undef _QSORT_FUNCTION_SUFFIX
