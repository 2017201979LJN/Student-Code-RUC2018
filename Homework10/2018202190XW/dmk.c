#include <time.h>
#include <math.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
union {
	double d;
	unsigned long long a;
}x;
unsigned long long gener() {
	unsigned long long ret = 0;
	ret |= rand();
	ret <<= 16;
	ret |= rand();
	ret <<= 16;
	ret |= rand();
	ret <<= 16;
	ret |= rand();
	return ret;
}
int main() {
	FILE *output = fopen("data.in", "w");
	srand(time(0));
	int n = 100000;
	fprintf(output, "%d\n", n);
	for (int i = 0; i < n; i++) {
		/*x.a = gener();
		while (isnan(x.d) || isinf(x.d)) x.a = gener();
		fprintf(output, "%.6le ", x.d);*/
		if (rand() % 2) fprintf(output, "%c", '-');
		fprintf(output, "%lld.%d%d%d ", gener() % 10000, rand() % 10, rand() % 10, rand() % 10);
		char c = rand() % 26 + 'a';
		fprintf(output, "%c\n", c);
	}
	return 0;
}
