#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#define N 100005
#define begin 0
#define dhalf 2
#define int_byte 4
#define ll_byte 8
#define type struct node
int n;
struct node {
	int x;
};
type a[N];
int cmp (const void *x, const void *y) {
	//printf("cmp: %d %d\n",*(type*)x, *(type*)y);
	if (((type *)x)->x < ((type *)y)->x) return -1;
	if (((type *)y)->x < ((type *)x)->x) return 1;
	return 0;
} 
void copy (char * iter, void * base, size_t size) {
	for (int i = 0; i < size / ll_byte; i++) {
		*(long long *)iter = *(long long *)(base + i * ll_byte);
		iter += ll_byte;
	}
	int pos = size / ll_byte * ll_byte, bit = size - pos;
	//printf ("pos=%d bit=%d\n", pos, bit);
	for (int i = 0; i < bit; i++) {
		*(char *)iter = *(char *)(base + pos + i);
		//printf ("%d",*(char*)iter);
		iter ++;
	}
	//printf ("%lf %lf\n",*(type *) base, *((type *)(iter-int_byte)));
	//puts("");
}
void copy_re (void * base, size_t size, char * be) {
	for (int i = 0; i < size / ll_byte; i++) {
		*(long long *)(base + i * ll_byte) = *(long long *)(be);
		be += ll_byte;
	}
	int pos = size / ll_byte * ll_byte, bit = size - pos;
	for (int i = 0; i < bit; i++) {
		*(char *)(base + pos + i) = *(char *)(be);
		be ++;
	}
	//printf ("basei=%d\n",*(int*)(base));
}
void solve (void * base, size_t l, size_t r, size_t size, int(*cmp)(const void *, const void *)) {
	if (l == r) return;
	int mid = (l + r) / dhalf;
	solve (base, l, mid, size, cmp);
	solve (base, mid + 1, r, size, cmp);
	//printf ("%lu %lu\n", l, r);
	int temp[(r - l + 1) * size / int_byte];
	char *be, *iter;
	be = (char *)temp; iter = (char *)temp;
	for (int i = l, j = mid + 1; i <= mid || j <= r;) {
		//printf ("%d\n", cmp ((char *)base + i * size, base + j * size));
		if (i > mid) {
			copy (iter, (char *)base + j * size, size);
			j++;
		}
		else if (j > r) {
			copy (iter, (char *)base + i * size, size);
			i++;
		}
		else {
			if (cmp ((char *)base + i * size, (char *)base + j * size) <= 0) {
				copy (iter, (char *)base + i * size, size);
				i++;
			}
			else {
				copy (iter, (char *)base + j * size, size);
				j++;
			}
		}
		iter += size;
		//printf ("v=%lf ", *(type *)(iter - size));
	}
	for (int i = l; i <= r; i++) {
		copy_re ((char *)base + i * size, size, be);
		be += size;
	}
	/*for (int i = l; i <= r; i++)
		printf ("%lf ", *(type *)(base + i * size));
	puts("");*/
}
void turing_qsort (void * base, size_t nmemb, size_t size, int(*cmp)(const void *, const void *)) {
	solve (base, begin, nmemb - 1, size, cmp);
}
int main () {
	freopen ("turing_qsort.in", "r", stdin);
	freopen ("turing_qsort.out", "w", stdout);
	scanf ("%d", &n);
	for (int i = 0; i < n; i++)
		scanf ("%d", &a[i].x);
	turing_qsort (a, n, sizeof (type), &cmp);
	//qsort (a, n, sizeof (type), &cmp);
	for (int i = 0; i < n; i++)
		printf ("%d ", a[i].x);
	//printf ("%ld\n", clock());
	return 0;
}
