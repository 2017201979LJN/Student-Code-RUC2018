#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// for IO
#define OUTPUT "data.in"
#define WRITE_ONLY "w"

// limits
#define NUM_OF_ELEMENTS_MIN 1000
#define NUM_OF_ELEMENTS_MAX 10000

#define EPS 1e-10

void init(void);

int gen_rand_int_limit(int L, int R);

char gen_rand_char(void);
int gen_rand_int(void);
double gen_rand_double(void);
double gen_rand_nonspecial_double(void);
long long gen_rand_longlong(void);

int is_NaN(double num);

int main(void)
{
	init();
	int num_of_elements = gen_rand_int_limit(NUM_OF_ELEMENTS_MIN,
											 NUM_OF_ELEMENTS_MAX);
	printf("%d\n", num_of_elements);
	for (int cnt = num_of_elements; cnt-- > 0;)
	{
		printf("%d%c", (int)gen_rand_char(), "\n "[cnt > 0]);
	}
	for (int cnt = num_of_elements; cnt-- > 0;)
	{
		printf("%d%c", gen_rand_int(), "\n "[cnt > 0]);
	}
	for (int cnt = num_of_elements; cnt-- > 0;)
	{
		printf("%.10f%c", gen_rand_nonspecial_double(), "\n "[cnt > 0]);
	}
	for (int cnt = num_of_elements; cnt-- > 0;)
	{
		int a;
		char b;
		long long c;
		char d;
		double e;
		a = gen_rand_int();
		b = gen_rand_char();
		c = gen_rand_longlong();
		d = gen_rand_char();
		e = gen_rand_double();
		printf("%d %d %lld %d %.10f\n", a, (int)b, c, (int)d, e);
	}
}

void init(void)
{
	srand(time(NULL));
	freopen(OUTPUT, WRITE_ONLY, stdout);
}

int gen_rand_int_limit(int L, int R)
{
	return L + rand() % (R - L + 1);
}

char gen_rand_char(void)
{
	return (char)(rand() % (UCHAR_MAX + 1));
}

int gen_rand_int(void)
{
	int res;
	char* ptr = (char*)&res;
	for (int i = 0; i < sizeof(int); ++i, ++ptr)
		*ptr = gen_rand_char();
	return res;
}

double gen_rand_double(void)
{
	double res;
	char* ptr = (char*)&res;
	for (int i = 0; i < sizeof(double); ++i, ++ptr)
		*ptr = gen_rand_char();
	return res;
}

double gen_rand_nonspecial_double(void)
{
	double res;
	do
	{
		res = gen_rand_double();
	}
	while (is_NaN(res));
	if (fabs(res) < EPS) res = 0.0;
	return res;
}

long long gen_rand_longlong(void)
{
	long long res;
	char* ptr = (char*)&res;
	for (int i = 0; i < sizeof(long long); ++i, ++ptr)
		*ptr = gen_rand_char();
	return res;
}

int is_NaN(double num)
{
	return !(num < 0) && !(num >= 0);
}

