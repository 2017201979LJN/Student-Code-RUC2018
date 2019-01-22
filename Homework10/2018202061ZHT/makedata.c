#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
int n;
int main() {
	freopen ("tree.in", "w", stdout);
	srand (time (0));
	n=100000;
	printf ("%d\n", n);
	for (int i = 1; i <= n; i++) {
		printf ("%d.%d\n", rand () % 1000, rand () % 1000);
		printf ("%d\n", rand () % 26);
	}
	return 0;
}
