#ifndef _CR_SORT_H

#define _CR_SORT_H

#include <stddef.h>

void cr_swap(void*, void*, size_t);

void cr_bubblesort(void*,
                   size_t,
                   size_t,
                   int (*)(const void*, const void*));

void cr_qsort(void*,
              size_t,
              size_t,
              int (*)(const void*, const void*));

#endif
