#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*typedef struct sth
{
    double a;
    int b;
} sth;
sth a[1010];*/

void swap(void *a, void *b, size_t size)
{
    static void *t = NULL;
    t = malloc(size);
    memcpy(t, a, size);
    memcpy(a, b, size);
    memcpy(b, t, size);
    free(t);
}

void mergesort(void *base, size_t nitems, size_t size, int (*compar)(const void *a, const void *b))
{
    if (nitems == 1)
        return;
    int mid = nitems >> 1;
    void *midd = base + size * mid, *end = base + size * nitems;
    mergesort(base, mid, size, compar);
    mergesort(midd, nitems - mid, size, compar);
    void *p = base;
    while (p != midd)
    {
        if (compar(p, midd) > 0)
        {
            swap(p, midd, size);
            if (midd + size != end)
            {
                for (void *p1 = midd, *p2 = midd + size; p2 != end && compar(p1, p2) > 0; swap(p1, p2, size), p1 += size, p2 += size)
                    ;
            }
        }
        p += size;
    }
}

/*int comp(const void *a, const void *b)
{
    if (((sth *)a)->a != ((sth *)b)->a) return (((sth *)a)->a < ((sth *)b)->a) ? -1 : 1;
    else return ((sth *)a)->b - ((sth *)b)->b;
}*/

int main()
{
    int n;
    scanf("%d", &n);
    /*for (int i = 1; i <= n; i++)
        scanf("%lf%d", &a[i].a, &a[i].b);
    mergesort(a + 1, n, sizeof(a[0]), comp);
    for (int i = 1; i <= n; i++)
        printf("%.5f %d\n", a[i].a, a[i].b);*/
    return 0;
}