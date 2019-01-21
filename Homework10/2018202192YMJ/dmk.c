#include <math.h>
#include <time.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

#define NUM_OF_DATA 100000
#define NUM_OF_BYTES 8

int main()
{
	srand(time(NULL));
	int n = NUM_OF_DATA;
	FILE* output = fopen("data.in", "w");
	fprintf(output, "%d\n", n);
	for (int i = 0; i < n; ++i)
	{
		char c = 'a' + rand() % 26;
		if (rand() & 1) putc('-', output);
		for (int j = 0; j < 4; ++j)
		{
			putc('0' + rand() % 10, output);
		}
		putc('.', output);
		for (int j = 0; j < 3; ++j)
		{
			putc('0' + rand() % 10, output);
		}
		putc(' ', output);
		fprintf(output, "%c\n", c);
	}
	fclose(output);
	return 0;
}
