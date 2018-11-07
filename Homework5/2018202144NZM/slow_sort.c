#include <stdio.h>
#include <string.h>

#define SIZE_OF_BYTES 8
#define MAX_ELE_SIZE 

#define min(a,b) (a) < (b) ? (a) : (b)

int aa[10000];

int cmp (void *a, void *b) {
    return *(int *)a - *(int *)b;
}

static inline void swap_t (void *v1, void *v2, int ele_size)
{
    if (ele_size == 1) {
        char temp = *(char *)v1;
        *(char *)v1 = *(char *)v2;
        *(char *)v2 = temp;
    }
    else if (ele_size == 2) {
        short temp = *(short *)v1;
        *(short *)v1 = *(short *)v2;
        *(short *)v2 = temp;
    }
    else if (ele_size == 4) {
        int temp = *(int *)v1;
        *(int *)v1 = *(int *)v2;
        *(int *)v2 = temp;
    }
    else if (ele_size == 8) {
        long long temp = *(long long *)v1;
        *(long long *)v1 = *(long long *)v2;
        *(long long *)v2 = temp;
    }
    else
    {
        char *tail = v1 + ele_size, *head1 = v1, *head2 = v2;
        for ( ; head1 + SIZE_OF_BYTES <= tail; head1 += SIZE_OF_BYTES, head2 += SIZE_OF_BYTES) {
            long long temp = *(long long *)head1;
            *(long long *)head1 = *(long long *)head2;
            *(long long *)head2 = temp;
        }
        if (head1 != tail) {
            int temp = *(int *)head1;
            *(int *)head1 = *(int *)head2;
            *(int *)head2 = temp;
        }
    }
}

static inline char *getmid (char *l, char *mid, char *r, int (*cmp) (const void *, const void *))
{
    return cmp(l, mid) < 0 ? (cmp(mid, r) < 0 ? mid : (cmp(l, r) < 0 ? r : l)) : (cmp(mid, r) > 0 ? mid : (cmp(l, r) < 0 ? l : r));
}

void slow_sort (void *a, size_t arr_size, size_t ele_size, int (*cmp) (const void *, const void *) )
{
    char *head, *tail, *head1, *tail1, *head2, *tail2, *maxp, *mid;
    int swap_size, block_size, cmp_res, swap_cnt;
    loop:
         if (arr_size < 7) {
            maxp = (char *)a + arr_size * ele_size;
            for (head = (char *)a + ele_size; head < maxp; head += ele_size)
                for (tail = head; tail > (char *)a && cmp(tail - ele_size, tail) > 0; tail -= ele_size)
                    swap_t(tail - ele_size, tail, ele_size);
            return;
         }
         mid = (char *)a + (arr_size / 2) * ele_size;
         if (arr_size > 7) {
            head = (char *)a;
            tail = (char *)a + (arr_size - 1) * ele_size;
            if (arr_size > 40) {
                block_size = arr_size / 8 * ele_size;
                head = getmid (head, head + block_size, head + block_size * 2, cmp),
                mid = getmid (mid - block_size, mid, mid + block_size, cmp);
                tail = getmid (tail - block_size * 2, tail - block_size, tail, cmp);
            }
            mid = getmid (head, mid, tail, cmp);
         }
         swap_t(a, mid, ele_size);
         head1 = (char *)a + ele_size, head2 = (char *)a + (arr_size - 1) * ele_size;
         tail1 = head1, tail2 = head2;
         swap_cnt = 0;
         while (1) {
            while (tail1 <= head2 && (cmp_res = cmp (tail1, a) ) <= 0) {
                if (cmp_res == 0) {
                    swap_t (head1, tail1, ele_size);
                    swap_cnt = 1;
                    head1 += ele_size;
                }
                tail1 += ele_size;
            }
            while (tail1 <= head2 && (cmp_res = cmp (head2, a) ) >= 0) {
                if (cmp_res == 0) {
                    swap_t (head2, tail2, ele_size);
                    swap_cnt = 1;
                    tail2 -= ele_size;
                }
                head2 -= ele_size;
            }
            if (tail1 > head2)
                break;
            swap_t (tail1, head2, ele_size);
            swap_cnt = 1;
            tail1 += ele_size;
            head2 -= ele_size;
         }
         tail = (char *)a + arr_size * ele_size;
         swap_size = min (head1 - (char *)a, tail1 - head1);
         swap_t (a, tail1 - swap_size, swap_size);
         swap_size = min (tail2 - head2, tail - tail2 - ele_size);
         swap_t (tail1, tail - swap_size, swap_size);
         if (swap_cnt == 0) {
            swap_size = 1 + arr_size / 4;
            for (mid = (char *)a + ele_size; mid < (char *)a + arr_size * ele_size; mid += ele_size) {
                for (head = mid; head > (char *)a && cmp (head, head - ele_size) < 0; head -= ele_size) {
                    swap_t (head, head - ele_size, ele_size);
                    if (++swap_cnt > swap_size)
                        goto insert_out;
                }
            }
         }
 insert_out:
         swap_size = tail1 - head1;
         if (swap_size > ele_size)
            slow_sort (a, swap_size / ele_size, ele_size, cmp);
         swap_size = tail2 - head2;
         if (swap_size > ele_size) {
            a = tail - swap_size;
            arr_size = swap_size / ele_size;
         }
     goto loop;
}

int main()
{
    freopen ("sort.in", "r", stdin);
    int n;
    scanf ("%d", &n);
    for (int i = 0; i < n; i++)
        scanf ("%d", aa + i);
    slow_sort (aa, n ,4, cmp);
    for (int i = 0; i < n; i++)
        printf ("%d ", aa[i]);
    return 0;
}
