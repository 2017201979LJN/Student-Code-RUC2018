#include <stdio.h>
#include "qtdlib.h"
#include <stdlib.h>
#define TOLERANT_N 100000

int a[110000], b[110000];
/*
struct Node {
	char a[6];
	short b[3];
	int c[5];
};

struct Node a[110000], b[110000];
*/
unsigned long long rdtsc()
{
    unsigned long long ret = 0;
    __asm__ __volatile__(
        "lfence\n\t"
        "mfence\n\t"
        "rdtsc\n\t"
        "shlq $32,%%rdx\n\t"
        "orq %%rax,%%rdx\n\t"
        "movq %%rdx,%0"
        : "=r"(ret)
        :
        : "%rax", "%rdx");
    return ret;
}

int cmp(const void *a, const void *b) {
	return *(int *)a - *(int *)b;
}
/*
int cmp(const void *a, const void *b) {
	struct Node *x = (struct Node *)a;
	struct Node *y = (struct Node *)b;
	if(x -> a[3] != y -> a[3]) return x -> a[3] - y -> a[3];
	if(x -> b[1] != y -> b[1]) return x -> b[1] - y -> b[1];
	return x -> c[4] - y -> c[4];
}
*/
int main() {
	int n;
	freopen("ssort.in", "r", stdin);
	scanf("%d", &n);
	for(int i = 1; i <= n; i++){
		scanf("%d", &a[i]);
		b[i] = a[i];
	}
/*	
	n = TOLERANT_N;
	struct Node *p = (struct Node *) malloc (sizeof(a[0]) * n);
	for(int i = 1; i <= n; i++){
		a[i] = *(p+i-1);
		b[i] = a[i];
	}
*/
	unsigned long long time_x = rdtsc();
	qsort(a+1, n, sizeof(a[0]), cmp);
	unsigned long long time_y = rdtsc();
	printf("%lld\n", time_y - time_x);
	ssort(b+1, n, sizeof(b[0]), cmp);
	unsigned long long time_z = rdtsc();
	printf("%lld\n", time_z - time_y);

	freopen("ssort1.out", "w", stdout);
	for(int i = 1; i <= n; i += 10){
		for(int j = i; j <= i + 9 && j <= n; j++)
			printf("%12d", a[j]);
		printf("\n");
	}
	fclose(stdout);
	freopen("ssort2.out", "w", stdout);
	for(int i = 1; i <= n; i += 10){
		for(int j = i; j <= i + 9 && j <= n; j++)
			printf("%12d", b[j]);
		printf("\n");
	}
/*
	freopen("ssort1.out", "w", stdout);
	for(int i = 1; i <= n; i++) {
		for(int j = 0; j < 6; j++) 
			printf("%c", a[i].a[j]);
		for(int j = 0; j < 3; j++) 
			printf("%d", a[i].b[j]);
		for(int j = 0; j < 5; j++)
			printf("%d", a[i].c[j]);
		printf("\n");
	}
	fclose(stdout);
	freopen("ssort2.out", "w", stdout);
	for(int i = 1; i <= n; i++){
		for(int j = 0; j < 6; j++) 
			printf("%c", b[i].a[j]);
		for(int j = 0; j < 3; j++) 
			printf("%d", b[i].b[j]);
		for(int j = 0; j < 5; j++)
			printf("%d", b[i].c[j]);
		printf("\n");
	}
*/
	return 0;
}
