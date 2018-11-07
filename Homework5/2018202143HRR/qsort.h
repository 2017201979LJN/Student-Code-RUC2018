#ifndef _MENCI_QSORT_H
#define _MENCI_QSORT_H

#include <stddef.h>

typedef int (*compare_func_t)(const void *, const void *);

void _qsort(void *base,
            size_t n,
            size_t obj_size,
            compare_func_t compare_func);

#endif // _MENCI_QSORT_H
