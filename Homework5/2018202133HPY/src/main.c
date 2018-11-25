#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cr_sort.h"
#include "rdtsc.h"

#define N 100000

typedef void sort_func_t(void*, size_t, size_t, int (*)(const void*, const void*));

sort_func_t* getSort(const char* name)
{
#define REGSORTFUNC(x)         \
    if (strcmp(name, #x) == 0) \
        return x;
    REGSORTFUNC(qsort)
    REGSORTFUNC(cr_bubblesort)
    REGSORTFUNC(cr_qsort)

    fprintf(stderr, "Warning: Unknown function: %s at %s->%s#%d\n", name, __FILE__, __func__, __LINE__);
    return NULL;
}

typedef struct {
    int  x;
    char Q[8];
} record_t;

record_t newRecord_t(int x)
{
    record_t ret = { x };
    return ret;
}

int cmp(const void* a, const void* b)
{
    if (((record_t*)a)->x > ((record_t*)b)->x)
        return 1;
    else if (((record_t*)a)->x == ((record_t*)b)->x)
        return 0;
    else
        return -1;
}

int main(int argc, char** argv)
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <func_num> <seed>\n", argv[0]);
        exit(-1);
    }

    srand(atoi(argv[2]));

    record_t* precord = malloc(N * sizeof(record_t));
    cycle_t   a, b;

    for (int i = 0; i < N; i++) {
        precord[i] = newRecord_t(rand());
    }

    if (N <= 100) {
        for (int i = 0; i < N; i++) {
            printf("%d ", precord[i].x);
        }
        puts("");
    }

    a = rdtsc();

    getSort(argv[1])(precord, N, sizeof(record_t), cmp);

    b = rdtsc();

    printf("%llu\n", b - a);

    if (N <= 100) {
        for (int i = 0; i < N; i++) {
            printf("%d ", precord[i].x);
        }
        puts("");
    }

    for (int i = 1; i < N; i++) {
        assert(cmp(&precord[i - 1], &precord[i]) <= 0);
    }

    free(precord);

    return 0;
}
