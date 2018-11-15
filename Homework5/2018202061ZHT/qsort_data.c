#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
int main () {
	freopen ("turing_qsort.in", "w", stdout);
	srand (time (0));
	int n = 1000;
	printf ("%d\n", n);
	for (int i = 1; i <= n; i++) {
		int x = rand () % 1000 + 1;
		printf ("%d ",x);
	}
	return 0;
}
