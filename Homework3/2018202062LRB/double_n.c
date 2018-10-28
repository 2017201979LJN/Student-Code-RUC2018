#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NUM_OF_BYTES 8
#define BITS_OF_BIG_NUMBER_2 256
#define BITS_OF_BIG_NUMBER_10 1024
#define BITS_OF_EXP 11
#define BITS_OF_FRAC 52
#define NUM_OF_OPERAND 1010
#define NUM_OF_OPERATOR 1010
#define LENGTH_OF_OPERAND 1010

typedef unsigned char* pointer;

typedef struct double_n
{
	unsigned char bytes[NUM_OF_BYTES];
}double_n;
typedef struct big_number_2
{
	int bit[BITS_OF_BIG_NUMBER_2];
}big_number_2;
typedef struct big_number_10
{
	int bit[BITS_OF_BIG_NUMBER_10];
}big_number_10;
double_n stack_double_n[NUM_OF_OPERAND], stack_double_n_temp[NUM_OF_OPERAND];
double_n digit[11];
const int bias = (1 << BITS_OF_EXP - 1) - 1;
short stack_type[NUM_OF_OPERAND], stack_temp[NUM_OF_OPERAND];
char s[NUM_OF_OPERAND * LENGTH_OF_OPERAND], ss[BITS_OF_BIG_NUMBER_10];

void show_bytes_binary_n(double_n v)
{
	for (int i = 0; i < NUM_OF_BYTES; i++)
	{
		for (int j = 7; j >= 0; j--) printf("%d", (v.bytes[i] >> j) & 1);
		puts("");
	}
	printf("\n");
}

void show_bytes_binary(pointer start, int len)
{
	for (int i = len - 1; i >= 0; i--)
	{
		for (int j = 7; j >= 0; j--)
			printf("%d", (start[i] >> j) & 1);
		puts("");
	}
	printf("\n");
}

void swap_int(int *a, int *b)
{
	int t = *a;
	*a = *b;
	*b = t;
}

void swap_long_long(long long *a,long long *b)
{
	long long t = *a;
	*a = *b;
	*b = t;
}

void swap_double_n(double_n *a, double_n *b)
{
	double_n t = *a;
	*a = *b;
	*b = t;
}

void swap_big_number_2(big_number_2 *a, big_number_2 *b)
{
	big_number_2 t = *a;
	*a = *b;
	*b = t;
}

void swap_big_number_10(big_number_10 *a, big_number_10 *b)
{
	big_number_10 t = *a;
	*a = *b;
	*b = t;
}

int bigger_big_number_2(big_number_2 a, big_number_2 b)
{
	if (a.bit[0] != b.bit[0]) return a.bit[0] < b.bit[0];
	int f = 0;
	for (int i = 1; i <= BITS_OF_BIG_NUMBER_2 - 1; i++)
	{
		if (a.bit[i] != b.bit[i])
		{
			f = a.bit[i] > b.bit[i];
			break;
		}
	}
	return f * (a.bit[0] == 0);
}

int bigger_big_number_10(big_number_10 a, big_number_10 b)
{
	if (a.bit[0] != b.bit[0]) return a.bit[0] < b.bit[0];
	int f = 0;
	for (int i = 1; i <= BITS_OF_BIG_NUMBER_10 - 1; i++)
	{
		if (a.bit[i] != b.bit[i])
		{
			f = a.bit[i] > b.bit[i];
			break;
		}
	}
	return f * (a.bit[0] == 0);
}

int smaller_big_number_2(big_number_2 a, big_number_2 b)
{
	if (a.bit[0] != b.bit[0]) return a.bit[0] > b.bit[0];
	int f = 0;
	for (int i = 1; i <= BITS_OF_BIG_NUMBER_2 - 1; i++)
	{
		if (a.bit[i] != b.bit[i])
		{
			f = a.bit[i] < b.bit[i];
			break;
		}
	}
	return f * (a.bit[0] == 0);
}

int smaller_big_number_10(big_number_10 a, big_number_10 b)
{
	if (a.bit[0] != b.bit[0]) return a.bit[0] > b.bit[0];
	int f = 0;
	for (int i = 1; i <= BITS_OF_BIG_NUMBER_10 - 1; i++)
	{
		if (a.bit[i] != b.bit[i])
		{
			f = a.bit[i] < b.bit[i];
			break;
		}
	}
	return f * (a.bit[0] == 0);
}

big_number_2 plus_big_number_2(big_number_2 a, big_number_2 b)
{
	big_number_2 ans;
	memset(ans.bit, 0, sizeof(ans.bit));
	int type = a.bit[0] ^ b.bit[0];
	if (type == 0)
	{
		ans.bit[0] = a.bit[0];
		for (int i = BITS_OF_BIG_NUMBER_2 - 1; i > 0; i--)
		{
			ans.bit[i] += a.bit[i] + b.bit[i];
			if (ans.bit[i] >= 2)
			{
				if (i != 1) ans.bit[i - 1]++;
				ans.bit[i] -= 2;
			}
		}
	}
	else
	{
		big_number_2 ta = a, tb = b;
		ta.bit[0] = 0;
		tb.bit[0] = 0;
		if (smaller_big_number_2(ta, tb)) swap_big_number_2(&a, &b);
		ans.bit[0] = a.bit[0];
		for (int i = BITS_OF_BIG_NUMBER_2 - 1; i > 0; i--)
		{
			ans.bit[i] += a.bit[i] - b.bit[i];
			if (ans.bit[i] < 0)
			{
				if (i != 1) ans.bit[i - 1]--;
				ans.bit[i] += 2;
			}
		}
	}
	return ans;
}

big_number_10 plus_big_number_10(big_number_10 a, big_number_10 b)
{
	big_number_10 ans;
	memset(ans.bit, 0, sizeof(ans.bit));
	int type = a.bit[0] ^ b.bit[0];
	if (type == 0)
	{
		ans.bit[0] = a.bit[0];
		for (int i = BITS_OF_BIG_NUMBER_10 - 1; i > 0; i--)
		{
			ans.bit[i] += a.bit[i] + b.bit[i];
			if (ans.bit[i] >= 10)
			{
				if (i != 1) ans.bit[i - 1]++;
				ans.bit[i] -= 10;
			}
		}
	}
	else
	{
		big_number_10 ta = a, tb = b;
		ta.bit[0] = 0;
		tb.bit[0] = 0;
		if (smaller_big_number_10(ta, tb)) swap_big_number_10(&a, &b);
		ans.bit[0] = a.bit[0];
		for (int i = BITS_OF_BIG_NUMBER_10 - 1; i > 0; i--)
		{
			ans.bit[i] += a.bit[i] - b.bit[i];
			if (ans.bit[i] < 0)
			{
				if (i != 1) ans.bit[i - 1]--;
				ans.bit[i] += 10;
			}
		}
	}
	return ans;
}

big_number_2 minus_big_number_2(big_number_2 a, big_number_2 b)
{
	b.bit[0] ^= 1;
	return plus_big_number_2(a, b);
}

big_number_10 minus_big_number_10(big_number_10 a, big_number_10 b)
{
	b.bit[0] ^= 1;
	return plus_big_number_10(a, b);
}

big_number_2 multiply_big_number_2(big_number_2 a, big_number_2 b)
{
	big_number_2 ans;
	memset(ans.bit, 0, sizeof(ans.bit));
	ans.bit[0] = a.bit[0] ^ b.bit[0];
	for (int i = BITS_OF_BIG_NUMBER_2 - 1; i > 0; i--)
	{
		for (int j = BITS_OF_BIG_NUMBER_2 - 1; j > 0; j--)
		{
			int t = BITS_OF_BIG_NUMBER_2 - 1 - i;
			if (j > t) ans.bit[j - t] += a.bit[i] * b.bit[j];
		}
	}
	for (int i = BITS_OF_BIG_NUMBER_2 - 1; i > 0; i--)
	{
		if (ans.bit[i] >= 2)
		{
			if (i != 1) ans.bit[i - 1] += ans.bit[i] / 2;
			ans.bit[i] &= 1;
		}
	}
	return ans;
}

big_number_10 multiply_big_number_10(big_number_10 a, big_number_10 b)
{
	big_number_10 ans;
	memset(ans.bit, 0, sizeof(ans.bit));
	ans.bit[0] = a.bit[0] ^ b.bit[0];
	for (int i = BITS_OF_BIG_NUMBER_10 - 1; i > 0; i--)
	{
		for (int j = BITS_OF_BIG_NUMBER_10 - 1; j > 0; j--)
		{
			int t = BITS_OF_BIG_NUMBER_10 - 1 - i;
			if (j > t) ans.bit[j - t] += a.bit[i] * b.bit[j];
		}
	}
	for (int i = BITS_OF_BIG_NUMBER_10 - 1; i > 0; i--)
	{
		if (ans.bit[i] >= 10)
		{
			if (i != 1) ans.bit[i - 1] += ans.bit[i] / 10;
			ans.bit[i] %= 10;
		}
	}
	return ans;
}

big_number_2 multiply2_big_number_2(big_number_2 a, int b) 
{
	big_number_2 ans = a;
	for (int i = BITS_OF_BIG_NUMBER_2 - 1; i > 0; i--) ans.bit[i] *= b;
	for (int i = BITS_OF_BIG_NUMBER_2 - 1; i > 0; i--)
	{
		if (ans.bit[i] >= 2)
		{
			if (i != 1) ans.bit[i - 1] += ans.bit[i] / 2;
			ans.bit[i] &= 1;
		}
	}
	return ans;
}

big_number_10 multiply2_big_number_10(big_number_10 a, int b) 
{
	big_number_10 ans = a;
	for (int i = BITS_OF_BIG_NUMBER_10 - 1; i > 0; i--) ans.bit[i] *= b;
	for (int i = BITS_OF_BIG_NUMBER_10 - 1; i > 0; i--)
	{
		if (ans.bit[i] >= 10)
		{
			if (i != 1) ans.bit[i - 1] += ans.bit[i] / 10;
			ans.bit[i] %= 10;
		}
	}
	return ans;
}

big_number_2 shl_2(big_number_2 v, int l)
{
	big_number_2 ans;
	memset(ans.bit, 0, sizeof(ans.bit));
	ans.bit[0] = v.bit[0];
	for (int i = l + 1; i <= BITS_OF_BIG_NUMBER_2 - 1; i++)
		ans.bit[i - l] = v.bit[i];
	return ans;
}

big_number_10 shl_10(big_number_10 v, int l)
{
	big_number_10 ans;
	memset(ans.bit, 0, sizeof(ans.bit));
	ans.bit[0] = v.bit[0];
	for (int i = l + 1; i <= BITS_OF_BIG_NUMBER_10 - 1; i++)
		ans.bit[i - l] = v.bit[i];
	return ans;
}

big_number_2 shr_2(big_number_2 v, int l)
{
	big_number_2 ans;
	memset(ans.bit, 0, sizeof(ans.bit));
	ans.bit[0] = v.bit[0];
	for (int i = BITS_OF_BIG_NUMBER_2 - 1 - l; i > 0; i--)
		ans.bit[i + l] = v.bit[i];
	return ans;
}

big_number_10 shr_10(big_number_10 v, int l)
{
	big_number_10 ans;
	memset(ans.bit, 0, sizeof(ans.bit));
	ans.bit[0] = v.bit[0];
	for (int i = BITS_OF_BIG_NUMBER_10 - 1 - l; i > 0; i--)
		ans.bit[i + l] = v.bit[i];
	return ans;
}

big_number_2 divide_big_number_2(big_number_2 a, big_number_2 b)
{
	big_number_2 ans;
	memset(ans.bit, 0, sizeof(ans.bit));
	int minus = a.bit[0] ^ b.bit[0];
	a.bit[0] = 0, b.bit[0] = 0;
	a = shl_2(a, BITS_OF_BIG_NUMBER_2 - 1 - BITS_OF_FRAC - 2);
	for (int i = BITS_OF_FRAC + 2; i <= BITS_OF_BIG_NUMBER_2 - 1; i++)
	{
		ans.bit[i] = 1;
		if (bigger_big_number_2(shl_2(b, BITS_OF_BIG_NUMBER_2 - 1 - i), a)) ans.bit[i] = 0;
		if (ans.bit[i] == 1) a = minus_big_number_2(a, shl_2(b, BITS_OF_BIG_NUMBER_2 - 1 - i));
	}
	ans = shr_2(ans, BITS_OF_BIG_NUMBER_2 - 1 - 3 * BITS_OF_FRAC - 2);
	ans.bit[0] = minus;
	return ans;
}

big_number_10 divide_big_number_10(big_number_10 a, big_number_10 b)
{
	big_number_10 ans;
	memset(ans.bit, 0, sizeof(ans.bit));
	int minus = a.bit[0] ^ b.bit[0];
	a.bit[0] = 0, b.bit[0] = 0;
	for (int i = BITS_OF_FRAC + 2; i < BITS_OF_BIG_NUMBER_10; i++)
	{
		int j = 1;
		while ((!bigger_big_number_10(multiply2_big_number_10(shl_10(b, BITS_OF_BIG_NUMBER_10 - 1 - i), j), a)) && j <= 9) j++;
		ans.bit[i] = j - 1;
		if (ans.bit[i] > 0)
		{
			a = minus_big_number_10(a,
				multiply2_big_number_10(shl_10(b, BITS_OF_BIG_NUMBER_10 - 1 - i), ans.bit[i]));
		}
	}
	ans.bit[0] = minus;
	return ans;
}

big_number_10 divide2_big_number_10(big_number_10 a, int b) 
{
	big_number_10 ans;
	memset(ans.bit, 0, sizeof(ans.bit));
	for (int i = 1; i < BITS_OF_BIG_NUMBER_10; i++)
	{
		ans.bit[i] = a.bit[i] / b;
		if (i < BITS_OF_BIG_NUMBER_10 - 1) a.bit[i + 1] += (a.bit[i] % b) * 10;
	}
	return ans;
}

int equal_big_number_10(big_number_10 a, big_number_10 b)
{
	int zero = 1;
	for (int i = 1; i < BITS_OF_BIG_NUMBER_10; i++)
	{
		if (a.bit[i] != b.bit[i]) return 0;
		if (a.bit[i] != 0) zero = 0;
	}
	return (a.bit[0] == b.bit[0] || zero);
}

big_number_2 long_long_to_big_number_2(long long v)
{
	big_number_2 ans;
	memset(ans.bit, 0, sizeof(ans.bit));
	ans.bit[0] = v < 0;
	if (v < 0) v = -v;
	for (int i = BITS_OF_BIG_NUMBER_2 - 1; i > 0; i--)
	{
		ans.bit[i] = v & 1;
		v >>= 1;
	}
	return ans;
}

big_number_10 long_long_to_big_number_10(long long v)
{
	big_number_10 ans;
	memset(ans.bit, 0, sizeof(ans.bit));
	ans.bit[0] = v < 0;
	if (v < 0) v = -v;
	for (int i = BITS_OF_BIG_NUMBER_10 - 1; i > 0; i--)
	{
		ans.bit[i] = v % 10;
		v /= 10;
	}
	return ans;
}

double double_n_to_double(double_n v)
{
	unsigned long long ans = 0;
	for (int i = 0; i <= 7; i++)
	{
		ans = (ans << 8) + v.bytes[i];
	}
	return *(double *)&ans;
}

double_n make_double_n(int minus, int exp, long long frac)
{
	double_n ans;
	memset(ans.bytes, 0, sizeof(ans.bytes));
	ans.bytes[0] += (1 << 7) * minus;
	for (int i = 2; i <= BITS_OF_EXP + 1; i++)
	{
		int belong = (i - 1) / 8, bit = (i - 1) % 8 + 1;
		ans.bytes[belong] += (1 << (8 - bit)) * ((exp >> BITS_OF_EXP - i + 1) & 1);
	}
	for (int i = BITS_OF_EXP + 2; i <= NUM_OF_BYTES << 3; i++)
	{
		int belong = (i - 1) / 8, bit = (i - 1) % 8 + 1;
		ans.bytes[belong] += (1 << (8 - bit)) * ((frac >> BITS_OF_FRAC - i + BITS_OF_EXP + 1) & 1);
	}
	return ans;
}

int exp_of_double_n(double_n v)
{
	int ans = 0;
	for (int i = 2; i <= BITS_OF_EXP + 1; i++)
	{
		int belong = (i - 1) / 8, bit = (i - 1) % 8 + 1;
		ans = (ans << 1) + ((v.bytes[belong] >> (8 - bit)) & 1);
	}
	return ans;
}

long long frac_of_double_n(double_n v)
{
	long long ans = 0;
	for (int i = BITS_OF_EXP + 2; i <= NUM_OF_BYTES << 3; i++)
	{
		int belong = (i - 1) / 8, bit = (i - 1) % 8 + 1;
		ans = (ans << 1) + ((v.bytes[belong] >> (8 - bit)) & 1);
	}
	return ans;
}

int get_type(double_n v)
{
	int exp = exp_of_double_n(v), type = 0;//0:nor  1:den  2:INF  3:NaN
	long long frac = frac_of_double_n(v);
	if (exp != 0)
	{
		if (exp != (1 << BITS_OF_EXP) - 1) type = 0;
		else type = 2 + (frac != 0);
	}
	else type = 1;
	return type;
}

big_number_10 qpow_big_number_10(big_number_10 v, int p)
{
	big_number_10 ans = long_long_to_big_number_10(1), t = v;
	while (p > 0)
	{
		if (p & 1) ans = multiply_big_number_10(ans, t);
		p >>= 1;
		t = multiply_big_number_10(t, t);
	}
	return ans;
}

big_number_10 getnum(int v)
{
	big_number_10 ans;
	memset(ans.bit, 0, sizeof(ans.bit));
	ans.bit[(BITS_OF_BIG_NUMBER_10 >> 1) - 1] = v;
	return ans;
}

double_n big1(double_n v)
{
	int minus = (v.bytes[0] >> 7) & 1;
	int exp = exp_of_double_n(v), type = get_type(v);
	long long frac = frac_of_double_n(v);
	if (type >= 2) return v;
	if (frac == (1LL << BITS_OF_FRAC) - 1) return make_double_n(minus, exp + 1, 0);
	else return make_double_n(minus, exp, frac + 1);
}

big_number_10 double_n_to_big_number_10(double_n v)
{
	int exp = exp_of_double_n(v), type = get_type(v);
	long long frac = frac_of_double_n(v);
	if (type == 0) frac += 1LL << BITS_OF_FRAC;
	else exp--;
	if ((v.bytes[0] >> 7) & 1 == 1) frac = -frac;
	big_number_10 ans = long_long_to_big_number_10(frac), mul,
		num2 = long_long_to_big_number_10(2);
	int point = BITS_OF_BIG_NUMBER_10 >> 1;
	ans = shl_10(ans, point);
	int E = exp - bias;
	if (E > 0)
	{
		mul = qpow_big_number_10(num2, E);
		ans = multiply_big_number_10(ans, mul);
	}
	else
	{
		mul = qpow_big_number_10(num2, -E);
		ans = divide_big_number_10(ans, mul);
	}
	ans = divide_big_number_10(ans, long_long_to_big_number_10(1LL << BITS_OF_FRAC));
	return ans;
}

double_n rounding(double_n a, big_number_10 v)
{
	double_n b = big1(a);
	int point = BITS_OF_BIG_NUMBER_10 >> 1;
	big_number_10 va = shl_10(minus_big_number_10(v, double_n_to_big_number_10(a)), point),
	              vb = shl_10(minus_big_number_10(double_n_to_big_number_10(b), v), point);
	va.bit[0] = vb.bit[0] = 0;
	if (bigger_big_number_10(va, vb)) return b;
	else return a;
}

double_n big_number_10_to_double_n(big_number_10 v)
{
	big_number_10 temp = v;
	if (equal_big_number_10(v, getnum(0))) return make_double_n(0, 0, 0LL);
	int minus = v.bit[0];
	v.bit[0] = 0;
	int E = 0;
	if (bigger_big_number_10(v ,getnum(1)))
	{
		while (!smaller_big_number_10(v, getnum(2)) && E < bias)
			v = divide2_big_number_10(v, 2), E++;
	}
	else
	{
		while (smaller_big_number_10(v, getnum(1)) && E > -bias)
			v = multiply2_big_number_10(v, 2), E--;
	}
	if (!smaller_big_number_10(v, getnum(2))) return make_double_n(minus, (1 << BITS_OF_EXP) - 1, 0LL);
	long long frac = 0;
	if (v.bit[(BITS_OF_BIG_NUMBER_10 >> 1) - 1] == 1)
	{
		frac = 1;
		v.bit[(BITS_OF_BIG_NUMBER_10 >> 1) - 1] = 0;
	}
	for (int i = 1; i <= BITS_OF_FRAC; i++)
	{
		v = multiply2_big_number_10(v, 2);
		int small = v.bit[(BITS_OF_BIG_NUMBER_10 >> 1) - 1] == 0;
		frac = (frac << 1) + (!small);
		if (!small) v.bit[(BITS_OF_BIG_NUMBER_10 >> 1) - 1] = 0;
	}
	int exp = E + bias;
	if (exp == 1) frac -= 1LL << BITS_OF_FRAC;
	//return make_double_n(minus, exp, frac);
	return rounding(make_double_n(minus, exp, frac), temp);
}

void print_double_n(double_n v)
{
	int exp = exp_of_double_n(v), type = get_type(v);
	long long frac = frac_of_double_n(v);
	if (exp == (1 << BITS_OF_EXP) - 1)
	{
		(frac == 0) ? printf("inf") : printf("nan");
		return;
	}
	big_number_10 ans = double_n_to_big_number_10(v);
	if (ans.bit[0] == 1) printf("-");
	int i = 1, point = BITS_OF_BIG_NUMBER_10 >> 1;
	while (ans.bit[i] == 0 && i < point) i++;
	if (i == point) printf("0");
	else
	{
		for (; i < point; i++) printf("%d", ans.bit[i]);
	}
	int j = BITS_OF_BIG_NUMBER_10 - 1;
	while (ans.bit[j] == 0 && j >= point) j--;
	if (j < point) return;
	else
	{
		printf(".");
		for (; i <= j; i++) printf("%d", ans.bit[i]);
	}
}

double_n plus(double_n a,double_n b)
{
	int type_a = get_type(a), type_b = get_type(b);
	if (type_a > type_b)
	{
		swap_double_n(&a, &b);
		swap_int(&type_a, &type_b);
	}
	if (type_b == 3) return b;
	else if (type_a == 2)
	{
		double_n ans = a;
		if ((a.bytes[0] >> 7) ^ (b.bytes[0] >> 7) == 1) ans.bytes[7]++;
		return ans;
	}
	else if (type_a < 2 && type_b == 2) return b;
	else
	{
		int exp_a = exp_of_double_n(a), exp_b = exp_of_double_n(b);
		big_number_2 frac_a = long_long_to_big_number_2(frac_of_double_n(a)),
				     frac_b = long_long_to_big_number_2(frac_of_double_n(b));
		if (type_a == 0) frac_a.bit[BITS_OF_BIG_NUMBER_2 - 1 - BITS_OF_FRAC]++;
		else exp_a--;
		if (type_b == 0) frac_b.bit[BITS_OF_BIG_NUMBER_2 - 1 - BITS_OF_FRAC]++;
		else exp_b--;
		if (a.bytes[0] >> 7 == 1) frac_a.bit[0] = 1;
		if (b.bytes[0] >> 7 == 1) frac_b.bit[0] = 1;
		if (exp_a < exp_b)
		{
			swap_double_n(&a, &b);
			swap_int(&exp_a, &exp_b);
			swap_big_number_2(&frac_a, &frac_b);
		}
		if (exp_a - exp_b > BITS_OF_FRAC + 1) return a;
		frac_a = shl_2(frac_a, exp_a - exp_b);
		exp_b = exp_a;
		int base = 1;
		while (frac_a.bit[base] == 0 && base < BITS_OF_BIG_NUMBER_2 - 1 - BITS_OF_FRAC) base++;
		big_number_2 frac_Ans = plus_big_number_2(frac_a, frac_b);
		int minus = frac_Ans.bit[0];
		long long frac_ans = 0;
		for (int i = base - 1; i <= base + BITS_OF_FRAC; i++)
			frac_ans = (frac_ans << 1) + frac_Ans.bit[i];
		if (frac_Ans.bit[base + BITS_OF_FRAC + 1] == 1)
		{
			int equal_to_five = 1;
			for (int i = base + BITS_OF_FRAC + 2; i <= BITS_OF_BIG_NUMBER_2 - 1; i++)
			{
				if (frac_Ans.bit[i] != 0)
				{
					equal_to_five = 0;
					break;
				}
			}
			if (!equal_to_five) frac_ans++;
			else if (frac_ans & 1) frac_ans++;
		}
		int exp_ans = exp_a;
		while ((frac_ans >> BITS_OF_FRAC > 1 && exp_ans < (1 << BITS_OF_EXP) - 1) || exp_ans < 0)
		{
			frac_ans >>= 1;
			exp_ans++;
		}
		while (frac_ans >> BITS_OF_FRAC == 0 && exp_ans > 0)
		{
			frac_ans <<= 1;
			exp_ans--;
		}
		if(exp_ans >= 1 << BITS_OF_EXP)
			return make_double_n(minus, (1 << BITS_OF_EXP) - 1, 0LL);
		if (exp_ans == 0) frac_ans >>= 1;
		return make_double_n(minus, exp_ans, frac_ans);
	}
}

double_n minus(double_n a,double_n b)
{
	double_n t = b;
	t.bytes[0] ^= 1 << 7;
	return plus(a, t);
}

double_n multiply(double_n a,double_n b)
{
	int type_a = get_type(a), type_b = get_type(b);
	if (type_a > type_b)
	{
		swap_double_n(&a, &b);
		swap_int(&type_a, &type_b);
	}
	if (type_b == 3) return b;
	else if (type_a == 2)
	{
		double_n ans = a;
		if (ans.bytes[0] >> 7 & 1) ans.bytes[0] -= 1 << 7;
		if ((a.bytes[0] >> 7) ^ (b.bytes[0] >> 7) == 1)
			ans.bytes[0] = ans.bytes[0] + (1 << 7) * ((a.bytes[0] >> 7) ^ (b.bytes[0] >> 7));
		return ans;
	}
	else if (type_a < 2 && type_b == 2) return b;
	else
	{
		int exp_a = exp_of_double_n(a), exp_b = exp_of_double_n(b);
		big_number_2 frac_a = long_long_to_big_number_2(frac_of_double_n(a)),
				     frac_b = long_long_to_big_number_2(frac_of_double_n(b));
		if (type_a == 0) frac_a.bit[BITS_OF_BIG_NUMBER_2 - 1 - BITS_OF_FRAC]++;
		else
		{
			exp_a--;
			frac_a = shl_2(frac_a, 1);
		}
		if (type_b == 0) frac_b.bit[BITS_OF_BIG_NUMBER_2 - 1 - BITS_OF_FRAC]++;
		else
		{
			exp_b--;
			frac_b = shl_2(frac_b, 1);
		}
		big_number_2 frac_Ans = multiply_big_number_2(frac_a, frac_b);
		int minus = (a.bytes[0] >> 7) ^ (b.bytes[0] >> 7);
		long long frac_ans = 0;
		for (int i = BITS_OF_BIG_NUMBER_2 - 1 - 2 * BITS_OF_FRAC - 1; i <= BITS_OF_BIG_NUMBER_2 - 1 - BITS_OF_FRAC; i++)
			frac_ans = (frac_ans << 1) + frac_Ans.bit[i];
		int exp_ans = exp_a + exp_b - bias, dexp = 0;
		while ((frac_ans >> BITS_OF_FRAC + 1 > 1 && exp_ans < (1 << BITS_OF_EXP) - 2) || exp_ans < 0)
		{
			frac_ans = (frac_ans << 1) + frac_Ans.bit[BITS_OF_BIG_NUMBER_2 - 1 - BITS_OF_FRAC + 1 - dexp];
			exp_ans++;
			dexp++;
		}
		while (frac_ans >> BITS_OF_FRAC + 1 == 0 && exp_ans > -1)
		{
			frac_ans <<= 1;
			exp_ans--;
			dexp--;
		}
		if (dexp > 0) frac_Ans = shr_2(frac_Ans, dexp);
		else frac_Ans = shl_2(frac_Ans, -dexp);
		if(exp_ans >= 1 << BITS_OF_EXP)
			return make_double_n(minus, (1 << BITS_OF_EXP) - 1, 0LL);
		if (exp_ans == 0)
		{
			if (frac_Ans.bit[BITS_OF_BIG_NUMBER_2 - 1 - BITS_OF_FRAC - 1] == 1)
			{
				int equal_to_five = 1;
				for (int i = BITS_OF_BIG_NUMBER_2 - 1 - BITS_OF_FRAC; i <= BITS_OF_BIG_NUMBER_2 - 1; i++)
				{
					if (frac_Ans.bit[i] != 0)
					{
						equal_to_five = 0;
						break;
					}
				}
				if (!equal_to_five) frac_ans += 4;
				else if ((frac_ans >> 2) & 1) frac_ans += 4;
			}
			frac_ans = frac_ans >> 1;
		}
		else
		{
			if (frac_Ans.bit[BITS_OF_BIG_NUMBER_2 - 1 - BITS_OF_FRAC] == 1)
			{
				int equal_to_five = 1;
				for (int i = BITS_OF_BIG_NUMBER_2 - 1 - BITS_OF_FRAC + 1; i <= BITS_OF_BIG_NUMBER_2 - 1; i++)
				{
					if (frac_Ans.bit[i] != 0)
					{
						equal_to_five = 0;
						break;
					}
				}
				if (!equal_to_five) frac_ans += 2;
				else if ((frac_ans >> 1) & 1) frac_ans += 2;
			}
		}
		frac_ans >>= 1;
		exp_ans++;
		return make_double_n(minus, exp_ans, frac_ans);
	}
}

double_n divide(double_n a,double_n b)
{
	int type_a = get_type(a), type_b = get_type(b);
	if (type_a == 3) return a;
	else if (type_b == 3) return b;
	else if (type_a == 2)
	{
		double_n ans = a;
		if (type_b == 2)
		{
			if (ans.bytes[0] >> 7 & 1) ans.bytes[0] -= 1 << 7;
			if ((a.bytes[0] >> 7) ^ (b.bytes[0] >> 7) == 1)
				ans.bytes[0] = ans.bytes[0] + (1 << 7) * ((a.bytes[0] >> 7) ^ (b.bytes[0] >> 7));
			return ans;
		}
		else if (exp_of_double_n(b) == 0 && frac_of_double_n(b) == 0)
		{
			ans.bytes[7]++;
			return ans;
		}
	}
	else if (type_a < 2 && type_b == 2)
	{
		double_n ans;
		memset(ans.bytes, 0, sizeof(ans.bytes));
		ans.bytes[0] += (1 << 7) * ((a.bytes[0] >> 7) & 1);
		return ans;
	}
	else
	{
		int exp_a = exp_of_double_n(a), exp_b = exp_of_double_n(b);
		if (exp_b == 0 && frac_of_double_n(b) == 0)
		{
			int minus = (a.bytes[0] >> 7) ^ (b.bytes[0] >> 7);
			if (exp_a == 0 && frac_of_double_n(a) == 0)
				return make_double_n(minus, (1 << BITS_OF_EXP) - 1, 1LL);
			else
				return make_double_n(minus, (1 << BITS_OF_EXP) - 1, 0LL);
		}
		else if (exp_a == 0 && frac_of_double_n(a) == 0) return a;
		long long frac_A = frac_of_double_n(a),
				  frac_B = frac_of_double_n(b);
		big_number_2 frac_a = long_long_to_big_number_2(frac_of_double_n(a)),
				     frac_b = long_long_to_big_number_2(frac_of_double_n(b));
		if (type_a == 0) frac_a.bit[BITS_OF_BIG_NUMBER_2 - 1 - BITS_OF_FRAC]++;
		else
		{
			exp_a--;
			frac_a = shl_2(frac_a, 1);
		}
		if (type_b == 0) frac_b.bit[BITS_OF_BIG_NUMBER_2 - 1 - BITS_OF_FRAC]++;
		else
		{
			exp_b--;
			frac_b = shl_2(frac_b, 1);
		}
		big_number_2 frac_Ans = divide_big_number_2(frac_a, frac_b);
		int minus = (a.bytes[0] >> 7) ^ (b.bytes[0] >> 7);
		long long frac_ans = 0;
		for (int i = BITS_OF_BIG_NUMBER_2 - 1 - 2 * BITS_OF_FRAC - 1; i <= BITS_OF_BIG_NUMBER_2 - 1 - BITS_OF_FRAC; i++)
			frac_ans = (frac_ans << 1) + frac_Ans.bit[i];
		int exp_ans = exp_a - exp_b + bias, dexp = 0;
		while ((frac_ans >> BITS_OF_FRAC + 1 > 1 && exp_ans < (1 << BITS_OF_EXP) - 2) || exp_ans < 0)
		{
			frac_ans >>= 1;
			exp_ans++;
			dexp++;
		}
		while (frac_ans >> BITS_OF_FRAC + 1 == 0 && exp_ans > -1)
		{
			frac_ans = (frac_ans << 1) + frac_Ans.bit[BITS_OF_BIG_NUMBER_2 - 1 - BITS_OF_FRAC + 1 - dexp];
			exp_ans--;
			dexp--;
		}
		if (dexp > 0) frac_Ans = shr_2(frac_Ans, dexp);
		else frac_Ans = shl_2(frac_Ans, -dexp);
		if(exp_ans >= 1 << BITS_OF_EXP)
			return make_double_n(minus, (1 << BITS_OF_EXP) - 1, 0LL);
		if (exp_ans == 0)
		{
			if (frac_Ans.bit[BITS_OF_BIG_NUMBER_2 - 1 - BITS_OF_FRAC - 1] == 1)
			{
				int equal_to_five = 1;
				for (int i = BITS_OF_BIG_NUMBER_2 - 1 - BITS_OF_FRAC; i <= BITS_OF_BIG_NUMBER_2 - 1; i++)
				{
					if (frac_Ans.bit[i] != 0)
					{
						equal_to_five = 0;
						break;
					}
				}
				if (!equal_to_five) frac_ans += 4;
				else if ((frac_ans >> 2) & 1) frac_ans += 4;
			}
			frac_ans >>= 1;
		}
		else
		{
			if (frac_Ans.bit[BITS_OF_BIG_NUMBER_2 - 1 - BITS_OF_FRAC] == 1)
			{
				int equal_to_five = 1;
				for (int i = BITS_OF_BIG_NUMBER_2 - BITS_OF_FRAC; i <= BITS_OF_BIG_NUMBER_2 - 1; i++)
				{
					if (frac_Ans.bit[i] != 0)
					{
						equal_to_five = 0;
						break;
					}
				}
				if (!equal_to_five) frac_ans += 2;
				else if ((frac_ans >> 1) & 1) frac_ans += 2;
			}
		}
		frac_ans >>= 1;
		exp_ans++;
		return make_double_n(minus, exp_ans, frac_ans);
	}
}

void print_big_number_10(big_number_10 v)
{
	if (v.bit[0] == 1) printf("-");
	int i = 1;
	while (v.bit[i] == 0 && i < BITS_OF_BIG_NUMBER_10) i++;
	if (i == BITS_OF_BIG_NUMBER_10)
	{
		printf("0");
		return;
	}
	for (; i < BITS_OF_BIG_NUMBER_10; i++) printf("%d", v.bit[i]);
}

void init()
{
	digit[0] = make_double_n(0, 0, 0);
	digit[1] = make_double_n(0, bias, 0);
	digit[2] = make_double_n(0, bias + 1, 0);
	digit[3] = make_double_n(0, bias + 1, 1LL << BITS_OF_FRAC - 1);
	digit[4] = make_double_n(0, bias + 2, 0);
	digit[5] = make_double_n(0, bias + 2, 1LL << BITS_OF_FRAC - 2);
	digit[6] = make_double_n(0, bias + 2, 1LL << BITS_OF_FRAC - 1);
	digit[7] = make_double_n(0, bias + 2, (1LL << BITS_OF_FRAC - 1) + (1LL << BITS_OF_FRAC - 2));
	digit[8] = make_double_n(0, bias + 3, 0);
	digit[9] = make_double_n(0, bias + 3, 1LL << BITS_OF_FRAC - 3);
	digit[10] = make_double_n(0, bias + 3, 1LL << BITS_OF_FRAC - 2);
}

int main()
{
//	freopen("data.txt", "r", stdin);
//	freopen("double_n.ans", "w", stdout);
	init();
	while (~scanf(" %[^\n]", s))
	{
		int len = strlen(s), num_temp = 0, num = 0;
		int last_type = -2;
		for (int i = 0; i < len; )
		{
			while (isspace(s[i])) i++;
			if (isdigit(s[i]) || s[i] == '.' || ((s[i] == '-' || s[i] == '+') && last_type != -1))
			{	
				int minus_frac = (s[i] == '-'), div = 0;
				if (s[i] == '-' || s[i] == '+') i++;
				stack_type[++num] = -1;
				big_number_10 frac = long_long_to_big_number_10(0);
				if (isdigit(s[i]))
				{
					while (isdigit(s[i]))
					{
						frac = plus_big_number_10(shl_10(frac, 1), long_long_to_big_number_10(s[i] - '0'));
						i++;
					}
				}
				if (s[i] == '.')
				{
					i++;
					while (isdigit(s[i]))
					{
						frac = plus_big_number_10(shl_10(frac, 1), long_long_to_big_number_10(s[i] - '0'));
						div++;
						i++;
					}
				}
				if (s[i] == 'e' || s[i] == 'E')
				{
					int div2 = 0;
					i++;
					int minus_exp = (s[i] == '-');
					if (s[i] == '-' || s[i] == '+') i++;
					while (isdigit(s[i]))
					{
						div2 = div2 * 10 + s[i] - '0';
						i++;
					}
					if (minus_exp == 0) div2 = -div2;
					div += div2;
				}
				frac = shl_10(frac, BITS_OF_BIG_NUMBER_10 >> 1);
				if (div > 0) frac = shr_10(frac, div);
				else frac = shl_10(frac, -div);
				if (minus_frac == 1) frac.bit[0] = 1;
				stack_double_n[num] = big_number_10_to_double_n(frac);
				last_type = -1;
			}
			else
			{
				int op;
				switch (s[i])
				{
				case '+':
					op = 0;
					break;
				case '-':
					op = 1;
					break;
				case '*':
					op = 2;
					break;
				case '/':
					op = 3;
					break;
				case '(':
					op = 4;
					break;
				case ')':
					op = 5;
					break;
				default:
					puts("???");
				}
				if (op == 4)
					stack_temp[++num_temp] = op;
				else if (op == 5)
				{
					while (stack_temp[num_temp] != 4) stack_type[++num] = stack_temp[num_temp--];
					num_temp--;
				}
				else
				{
					while (stack_temp[num_temp] >> 1 >= op >> 1 && stack_temp[num_temp] != 4 && num_temp != 0)
						stack_type[++num] = stack_temp[num_temp--];
					stack_temp[++num_temp] = op;
				}
				i++;
				last_type = op;
			}
		}
		while (num_temp > 0)
		{
			if (stack_temp[num_temp] < 4) stack_type[++num] = stack_temp[num_temp];
			num_temp--;
		}
		/*for (int i = 1; i <= num; i++)
		{
			printf("i = %d stack_type[i] = %d\n", i, stack_type[i]);
			switch (stack_type[i])
			{
			case -1:
				printf("%.5f\n", double_n_to_double(stack_double_n[i]));
				break;
			case 0:
				puts("+");
				break;
			case 1:
				puts("-");
				break;
			case 2:
				puts("*");
				break;
			case 3:
				puts("/");
				break;
			default:
				puts("???");
			}
		}*/
		int num_stack = 0;
		for (int i = 1; i <= num; i++)
		{
			switch (stack_type[i])
			{
			case -1:
				stack_double_n_temp[++num_stack] = stack_double_n[i];
				break;
			case 0:
				stack_double_n_temp[num_stack - 1] =
					plus(stack_double_n_temp[num_stack - 1], stack_double_n_temp[num_stack]);
				num_stack--;
				break;
			case 1:
				stack_double_n_temp[num_stack - 1] =
					minus(stack_double_n_temp[num_stack - 1], stack_double_n_temp[num_stack]);
				num_stack--;
				break;
			case 2:
				stack_double_n_temp[num_stack - 1] =
					multiply(stack_double_n_temp[num_stack - 1], stack_double_n_temp[num_stack]);
				num_stack--;
				break;
			case 3:
				stack_double_n_temp[num_stack - 1] =
					divide(stack_double_n_temp[num_stack - 1], stack_double_n_temp[num_stack]);
				num_stack--;
				break;
			default:
				puts("???");
			}
		}
		print_double_n(stack_double_n_temp[1]);
		puts("");
	}
	//fclose(stdin);
	//fclose(stdout);
	return 0;
}
