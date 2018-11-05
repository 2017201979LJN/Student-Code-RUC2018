#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define OUTPUT "calculate.c"
#define WRITE_ONLY "w"

#define NUM_OF_BITS_IN_LONG 64

#define NUM_OF_ARGUMENTS_MAX 5

#define FREQUENCY_OF_IMMEDIATES 5

int gen_rand_int_range(int, int);
long gen_rand_long();

int gen_rand_int_range(int L, int R)
{
	return rand() % (R - L + 1) + L;
}

long gen_rand_long()
{
	long res = 0;
	for (int i = 0; i < NUM_OF_BITS_IN_LONG; ++i)
	{
		if (rand() & 1)
		{
			res |= 1L << i;
		}
	}
	return res;
}

int main()
{
	srand(time(NULL));
	freopen(OUTPUT, WRITE_ONLY, stdout);
	int n = gen_rand_int_range(1, NUM_OF_ARGUMENTS_MAX);
	int m = gen_rand_int_range(n, 5 * n);
	printf("long calculate(");
	for (int i = 0; i < n; ++i)
	{
		if (i > 0)
		{
			printf(", ");
		}
		printf("long x%d", i);
	}
	printf(")\n");
	printf("{\n");
	for (int i = 0; i < m; ++i)
	{
		
		printf("\tlong x%d = ", n + i);

		char operator = (rand() & 1) ? '+': '*'; 

		int left_operand, right_operand;
		if (rand() % FREQUENCY_OF_IMMEDIATES == 0) left_operand = -1;
		else left_operand = gen_rand_int_range(0, n + i - 1);
		if (left_operand != -1 && rand() % FREQUENCY_OF_IMMEDIATES == 0) right_operand = -1;
		else right_operand = gen_rand_int_range(0, n + i - 1);

		if (left_operand != -1) printf("x%d", left_operand);
		else printf("%ldL", gen_rand_long());
		printf(" %c ", operator);
		if (right_operand != -1) printf("x%d", right_operand);
		else printf("%ldL", gen_rand_long());
		printf(";\n");
	}
	printf("\treturn x%d;\n", n + m - 1);
	printf("}\n");
	return 0;
}

