#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
	srand( time(0) );
	freopen("data.in", "w", stdout);
	int n = 100000;
	printf("%d\n", n);
	for (int i = 1; i <= n; i++)
	{
		printf("%d.", rand() % 10000);
		printf("%d %d\n", rand() % 1000, rand() % 26); 
	}
	fclose(stdout);
}
