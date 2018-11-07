#include <stdio.h>
#include "qsort.h"

int compare_int(const void *a, const void *b) {
    return *(int *)a - *(int *)b;
}

// int *_a;

int main() {
    int n;
    scanf("%d", &n);

    int a[n];
    for (int i = 0; i < n; i++) scanf("%d", &a[i]);

    // _a = a;

    _qsort(a, n, sizeof(int), compare_int);

    for (int i = 0; i < n; i++) printf("%d%c", a[i], " \n"[i == n - 1]);
}
