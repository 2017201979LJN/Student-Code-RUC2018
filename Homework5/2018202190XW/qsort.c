#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#define STACK_SIZE 30
#define SHORT_SIZE 8
#define MAX_NEED_SORT_CNT 2
#define MAX_CHAR_VALUE 255
#define NUM_OF_ELEMENT_PER_CASE 10000000

struct rec {
	int a;
	char b;
	long long c;
	char d;
	double e;
};

void swap(char *a, char *b, size_t width);
void qsort_m(void *base, size_t num, size_t width, int (*cmp)(const void *, const void *));
void short_sort(char *a, char *b, size_t width, int (*cmp)(const void *, const void *));

int cmp_int(const void *a, const void *b) {
	if (*(int *)a > *(int *)b) return 1;
	if (*(int *)a < *(int *)b) return -1;
	return 0;
}
int cmp_long_long(const void *a, const void *b) {
	if (*(long long *)a > *(long long *)b) return 1;
	if (*(long long *)a < *(long long *)b) return -1;
	return 0;
}
int cmp_double(const void *a, const void *b) {
	if (*(double *)a > *(double *)b) return 1;
	if (*(double *)a < *(double *)b) return -1;
	return 0;
}
int cmp_char(const void *a, const void *b) {
	if (*(char *)a > *(char *)b) return 1;
	if (*(char *)a < *(char *)b) return -1;
	return 0;
}
int cmp_struct(const void *a, const void *b) {
	if ((*(struct rec *)a).a > (*(struct rec *)b).a) return 1;
	if ((*(struct rec *)a).a < (*(struct rec *)b).a) return -1;
	if ((*(struct rec *)a).b > (*(struct rec *)b).b) return 1;
	if ((*(struct rec *)a).b < (*(struct rec *)b).b) return -1;
	if ((*(struct rec *)a).c > (*(struct rec *)b).c) return 1;
	if ((*(struct rec *)a).c < (*(struct rec *)b).c) return -1;
	if ((*(struct rec *)a).d > (*(struct rec *)b).d) return 1;
	if ((*(struct rec *)a).d < (*(struct rec *)b).d) return -1;
	if ((*(struct rec *)a).e > (*(struct rec *)b).e) return 1;
	if ((*(struct rec *)a).e < (*(struct rec *)b).e) return -1;
	return 0;
}
int rand_int() {
	int ret;
	unsigned char *base = (unsigned char *)&ret;
	for (int i = 0; i < sizeof(int); i++) *(base + i) = rand() % MAX_CHAR_VALUE;
	return ret;
}
char rand_char() {
	char ret;
	unsigned char *base = (unsigned char *)&ret;
	for (int i = 0; i < sizeof(char); i++) *(base + i) = rand() % MAX_CHAR_VALUE;
	return ret;
}
long long rand_long_long() {
	long long ret;
	unsigned char *base = (unsigned char *)&ret;
	for (int i = 0; i < sizeof(long long); i++) *(base + i) = rand() % MAX_CHAR_VALUE;
	return ret;
}
double rand_double() {
	double ret;
	unsigned char *base = (unsigned char *)&ret;
	for (int i = 0; i < sizeof(double); i++) *(base + i) = rand() % MAX_CHAR_VALUE;
	if ((!(ret >= 0)) && (!(ret <= 0))) return rand_double();
	if (ret == 0) return 0;
	return ret;
}
struct rec rand_struct() {
	struct rec ret;
	ret.a = rand_int();
	ret.b = rand_char();
	ret.c = rand_long_long();
	ret.d = rand_char();
	ret.e = rand_double();
	return ret;
}
int main() {
	srand(time(0));
	struct timeval start, end;
	int timeuse;

	FILE *ans_int_m = fopen("ans_int_m.out", "w");
	FILE *ans_int_c = fopen("ans_int_c.out", "w");
	int *a_int = (int *)malloc(sizeof(int) * NUM_OF_ELEMENT_PER_CASE);
	int *b_int = (int *)malloc(sizeof(int) * NUM_OF_ELEMENT_PER_CASE);
	for (int i = 0; i < NUM_OF_ELEMENT_PER_CASE; i++) {
		*(a_int + i) = *(b_int + i) = rand_int();
	}
	gettimeofday(&start, NULL);
	qsort_m(a_int, NUM_OF_ELEMENT_PER_CASE, sizeof(int), cmp_int);
	gettimeofday(&end, NULL);
	timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
	printf("%.3lf\n", timeuse / 1000000.0);
	gettimeofday(&start, NULL);
	qsort(b_int, NUM_OF_ELEMENT_PER_CASE, sizeof(int), cmp_int);
	gettimeofday(&end, NULL);
	timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
	printf("%.3lf\n", timeuse / 1000000.0);
	for (int i = 0; i < NUM_OF_ELEMENT_PER_CASE; i++) {
		fprintf(ans_int_m, "%d ", a_int[i]);
		fprintf(ans_int_c, "%d ", b_int[i]);
	}
	free(a_int);
	free(b_int);
	fclose(ans_int_m);
	fclose(ans_int_c);	

	FILE *ans_double_m = fopen("ans_double_m.out", "w");
	FILE *ans_double_c = fopen("ans_double_c.out", "w");
	double *a_double = (double *)malloc(sizeof(double) * NUM_OF_ELEMENT_PER_CASE);
	double *b_double = (double *)malloc(sizeof(double) * NUM_OF_ELEMENT_PER_CASE);
	for (int i = 0; i < NUM_OF_ELEMENT_PER_CASE; i++) {
		*(a_double + i) = *(b_double + i) = rand_double();
	}
	gettimeofday(&start, NULL);
	qsort_m(a_double, NUM_OF_ELEMENT_PER_CASE, sizeof(double), cmp_double);
	gettimeofday(&end, NULL);
	timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
	printf("%.3lf\n", timeuse / 1000000.0);
	gettimeofday(&start, NULL);
	qsort(b_double, NUM_OF_ELEMENT_PER_CASE, sizeof(double), cmp_double);
	gettimeofday(&end, NULL);
	timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
	printf("%.3lf\n", timeuse / 1000000.0);
	for (int i = 0; i < NUM_OF_ELEMENT_PER_CASE; i++) {
		fprintf(ans_double_m, "%.5le ", a_double[i]);
		fprintf(ans_double_c, "%.5le ", b_double[i]);
	}
	free(a_double);
	free(b_double);
	fclose(ans_double_m);
	fclose(ans_double_c);
	
	FILE *ans_char_m = fopen("ans_char_m.out", "w");
	FILE *ans_char_c = fopen("ans_char_c.out", "w");
	char *a_char = (char *)malloc(sizeof(char) * NUM_OF_ELEMENT_PER_CASE);
	char *b_char = (char *)malloc(sizeof(char) * NUM_OF_ELEMENT_PER_CASE);
	for (int i = 0; i < NUM_OF_ELEMENT_PER_CASE; i++) {
		*(a_char + i) = *(b_char + i) = rand_char();
	}
	gettimeofday(&start, NULL);
	qsort_m(a_char, NUM_OF_ELEMENT_PER_CASE, sizeof(char), cmp_char);
	gettimeofday(&end, NULL);
	timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
	printf("%.3lf\n", timeuse / 1000000.0);
	gettimeofday(&start, NULL);
	qsort(b_char, NUM_OF_ELEMENT_PER_CASE, sizeof(char), cmp_char);
	gettimeofday(&end, NULL);
	timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
	printf("%.3lf\n", timeuse / 1000000.0);
	for (int i = 0; i < NUM_OF_ELEMENT_PER_CASE; i++) {
		fprintf(ans_char_m, "%c ", a_char[i]);
		fprintf(ans_char_c, "%c ", b_char[i]);
	}
	free(a_char);
	free(b_char);
	fclose(ans_char_m);
	fclose(ans_char_c);

	FILE *ans_struct_m = fopen("ans_struct_m.out", "w");
	FILE *ans_struct_c = fopen("ans_struct_c.out", "w");
	struct rec *a_struct = (struct rec *)malloc(sizeof(struct rec) * NUM_OF_ELEMENT_PER_CASE);
	struct rec *b_struct = (struct rec *)malloc(sizeof(struct rec) * NUM_OF_ELEMENT_PER_CASE);
	for (int i = 0; i < NUM_OF_ELEMENT_PER_CASE; i++) {
		*(a_struct + i) = *(b_struct + i) = rand_struct();
	}
	gettimeofday(&start, NULL);
	qsort_m(a_struct, NUM_OF_ELEMENT_PER_CASE, sizeof(struct rec), cmp_struct);
	gettimeofday(&end, NULL);
	timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
	printf("%.3lf\n", timeuse / 1000000.0);
	gettimeofday(&start, NULL);
	qsort(b_struct, NUM_OF_ELEMENT_PER_CASE, sizeof(struct rec), cmp_struct);
	gettimeofday(&end, NULL);
	timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
	printf("%.3lf\n", timeuse / 1000000.0);
	for (int i = 0; i < NUM_OF_ELEMENT_PER_CASE; i++) {
		fprintf(ans_struct_m, "%d %c %lld %c %.5le\n", a_struct[i].a, a_struct[i].b, a_struct[i].c, a_struct[i].d, a_struct[i].e);
		fprintf(ans_struct_c, "%d %c %lld %c %.5le\n", b_struct[i].a, b_struct[i].b, b_struct[i].c, b_struct[i].d, b_struct[i].e);
	}
	free(a_struct);
	free(b_struct);
	fclose(ans_struct_m);
	fclose(ans_struct_c);
	
	return 0;
}

void swap(char *a, char *b, size_t width) {
	char tmp;
	if (a != b) while (width--) {
		tmp = *a;
		*(a++) = *b;
		*(b++) = tmp;
	}
}

void qsort_m(void *base, size_t num, size_t width, int (*cmp)(const void *, const void *)) {
	int cnt = 0;
	char *l, *r, *mid, *l_id, *r_id;
	size_t size;
	int top;
	char *l_stack[STACK_SIZE], *r_stack[STACK_SIZE];
	if (num < MAX_NEED_SORT_CNT) return;
	top = 1;
	l_stack[top] = (char *)base;
	r_stack[top] = (char *)base + width * (num - 1);
	while (top) {
		l = l_stack[top];
		r = r_stack[top];
		top--;
		size = (r - l) / width + 1;
		if (size < SHORT_SIZE) {
			short_sort(l, r, width, cmp);
			continue;
		}
		mid = l + (size >> 1) * width;
		if (cmp(l, mid) > 0) {
			swap(l, mid, width);
		}
		if (cmp(l, r) > 0) {
			swap(l, r, width);
		}
		if (cmp(mid, r) > 0) {
			swap(mid, r, width);
		}
		l_id = l;
		r_id = r;
		for(;;) {
			while (l_id < mid && cmp(l_id, mid) <= 0) l_id += width;
			if (l_id == mid) l_id += width;
			while (l_id <= r && cmp(l_id, mid) <= 0) l_id += width;
			while (r_id > mid && cmp(r_id, mid) > 0) r_id -= width;
			if (r_id < l_id) break;
			swap(l_id, r_id, width);
			if (mid == r_id) mid = l_id;
		}
		while (r_id > mid && cmp(r_id, mid) == 0) r_id -= width;
		if (r_id == mid) r_id -= width;
		while (r_id >= l && cmp(r_id, mid) == 0) r_id -= width;
		if (r_id - l >= r - l_id) {
			if (l < r_id) {
				++top;
				l_stack[top] = l;
				r_stack[top] = r_id;
			}
			if (l_id < r) {
				++top;
				l_stack[top] = l_id;
				r_stack[top] = r;
			}
		}
		else {
			if (l_id < r) {
				++top;
				l_stack[top] = l_id;
				r_stack[top] = r;
			}
			if (l < r_id) {
				++top;
				l_stack[top] = l;
				r_stack[top] = r_id;
			}
		}
	}
}
void short_sort(char *l, char *r, size_t width, int (*cmp)(const void *, const void *)) {
	char *p, *max;
	while (l < r) {
		max = l;
		for (p = l + width; p <= r; p += width) {
			if (cmp(p, max) > 0) max = p;
		}
		swap(max, r, width);
		r -= width;
	}
}
