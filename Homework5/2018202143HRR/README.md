# qsort
An implanentation of C library function `qsort()` which is based on [Quick Sort](https://en.wikipedia.org/wiki/Quicksort) algorithm and falls back to [Insertion Sort](https://en.wikipedia.org/wiki/Insertion_sort) when the interval have few elements and falls back to [Heap Sort](https://en.wikipedia.org/wiki/Heapsort) when recursive too deep.

# Usage
Just `#include <qsort.h>` and use the function `_qsort()` to reaplce the C-library version of `qsort()`.

# Performance
Usually runs faster than `qsort()` when n = 400000 on my machine (Ubuntu 18.04 LTS on a Surface Book with Performance Base(i7-6600U)).

Obviously runs slower than `std::sort()`.
