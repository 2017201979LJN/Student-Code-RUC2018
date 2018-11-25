#include "cr_sort.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef likely
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#endif

extern inline void cr_swap(void* a, void* b, size_t siz)
{
    static void*  buf  = NULL;
    static size_t size = 0;

    if (unlikely(siz > size)) {
        size = siz;
        if (buf)
            free(buf);
        buf = malloc(siz);
    }

    memcpy(buf, a, siz);
    memcpy(a, b, siz);
    memcpy(b, buf, siz);
}

extern inline void cr_bubblesort(void*  base,
                                 size_t nmemb,
                                 size_t size,
                                 int (*compar)(const void*, const void*))
{
    void* end = base + nmemb * size;
    for (size_t i = 0; i < nmemb; i++) {
        for (void *p1 = base, *p2 = base + size; p2 != end; p1 += size, p2 += size) {
            if (compar(p1, p2) > 0) {
                cr_swap(p1, p2, size);
            }
        }
    }
}

void cr_qsort(void*  base,
              size_t nmemb,
              size_t size,
              int (*compar)(const void*, const void*))
{
    if (nmemb <= 0)
        return;
    if (nmemb <= 7) {
        cr_bubblesort(base, nmemb, size, compar);
        return;
    }

    void *begin  = base,
         *end    = base + nmemb * size,
         *mid    = base + (nmemb >> 1) * size;
    void* rbegin = end;

    {
        int cbm = compar(begin, mid);
        if (cbm == 0) {}
        else if (cbm > 0) {
            int cbe = compar(begin, rbegin);
            if (cbe == 0) {}
            else if (cbe > 0) {
                if (compar(mid, rbegin) >= 0) { // b > m >= e
                    cr_swap(begin, mid, size);
                }
                else { //b > e > m
                    cr_swap(begin, rbegin, size);
                }
            }
            else { // cbe < 0
                // e > b > m
            }
        }
        else { // cbm < 0
            int cbe = compar(begin, rbegin);
            if (cbe == 0) {}
            else if (cbe > 0) {
                // m > b > e
            }
            else { //cbe < 0
                if (compar(mid, rbegin) >= 0) {
                    // b < e <= mid
                    cr_swap(begin, rbegin, size);
                }
                else { // b < mid < e
                    cr_swap(begin, mid, size);
                }
            }
        }
    }

    begin += size;
    void* lend = begin;
    while (lend < rbegin) {
        while (lend < rbegin && compar(lend, base) <= 0)
            lend += size;
        while (lend < rbegin && compar(base, rbegin - size) <= 0)
            rbegin -= size;
        if (lend == rbegin)
            break;
        cr_swap(lend, rbegin - size, size);
    }
    lend -= size;

    cr_swap(base, lend, size);

    cr_qsort(base, (lend - base) / size, size, compar);
    cr_qsort(rbegin, (end - rbegin) / size, size, compar);
}
