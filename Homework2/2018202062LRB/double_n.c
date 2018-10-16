#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NUM_OF_BYTES 8
#define BITS_OF_EXP 11
#define BITS_OF_FRAC 52

typedef unsigned char* pointer;

typedef struct double_n
{
	unsigned char bytes[NUM_OF_BYTES];
}double_n;
typedef struct big_number
{
	int bit[256];
}big_number;
const int bias = (1 << BITS_OF_EXP - 1) - 1;

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

void swap_big_number(big_number *a, big_number *b)
{
	big_number t = *a;
	*a = *b;
	*b = t;
}

int bigger_big_number(big_number a, big_number b)
{
	if (a.bit[0] != b.bit[0]) return a.bit[0] < b.bit[0];
	int f = 0;
	for (int i = 1; i <= 255; i++)
	{
		if (a.bit[i] != b.bit[i])
		{
			f = a.bit[i] > b.bit[i];
			break;
		}
	}
	return f * (a.bit[0] == 0);
}

int smaller_big_number(big_number a, big_number b)
{
	if (a.bit[0] != b.bit[0]) return a.bit[0] > b.bit[0];
	int f = 0;
	for (int i = 1; i <= 255; i++)
	{
		if (a.bit[i] != b.bit[i])
		{
			f = a.bit[i] < b.bit[i];
			break;
		}
	}
	return f * (a.bit[0] == 0);
}

big_number plus_big_number(big_number a, big_number b)
{
	big_number ans;
	memset(ans.bit, 0, sizeof(ans.bit));
	int type = a.bit[0] ^ b.bit[0];
	if (type == 0)
	{
		ans.bit[0] = a.bit[0];
		for (int i = 255; i > 0; i--)
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
		big_number ta = a, tb = b;
		ta.bit[0] = 0;
		tb.bit[0] = 0;
		if (smaller_big_number(ta, tb)) swap_big_number(&a, &b);
		ans.bit[0] = a.bit[0];
		for (int i = 255; i > 0; i--)
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

big_number minus_big_number(big_number a, big_number b)
{
	b.bit[0] ^= 1;
	return plus_big_number(a, b);
}

big_number multiply_big_number(big_number a,big_number b)
{
	big_number ans;
	memset(ans.bit, 0, sizeof(ans.bit));
	ans.bit[0] = a.bit[0] ^ b.bit[0];
	for (int i = 255; i > 0; i--)
	{
		for (int j = 255; j > 0; j--)
		{
			int t = 255 - i;
			if (j > t) ans.bit[j - t] += a.bit[i] * b.bit[j];
		}
	}
	for (int i = 255; i > 0; i--)
	{
		if (ans.bit[i] >= 2)
		{
			if (i != 1) ans.bit[i - 1] += ans.bit[i] / 2;
			ans.bit[i] &= 1;
		}
	}
	return ans;
}

big_number shl(big_number v, int l)
{
	big_number ans;
	memset(ans.bit, 0, sizeof(ans.bit));
	ans.bit[0] = v.bit[0];
	for (int i = l + 1; i <= 255; i++)
		ans.bit[i - l] = v.bit[i];
	return ans;
}

big_number shr(big_number v, int l)
{
	big_number ans;
	memset(ans.bit, 0, sizeof(ans.bit));
	ans.bit[0] = v.bit[0];
	for (int i = 255 - l; i > 0; i--)
		ans.bit[i + l] = v.bit[i];
	return ans;
}

big_number divide_big_number(big_number a, big_number b)
{
	big_number ans;
	memset(ans.bit, 0, sizeof(ans.bit));
	int minus = a.bit[0] ^ b.bit[0], base = 1;
	a.bit[0] = 0, b.bit[0] = 0;
	a = shl(a, 255 - BITS_OF_FRAC - 2);//
	for (int i = BITS_OF_FRAC + 2; i <= 255; i++)
	{
		ans.bit[i] = 1;
		if (bigger_big_number(multiply_big_number(ans, b), a)) ans.bit[i] = 0;
	}
	ans = shr(ans, 255 - 3 * BITS_OF_FRAC - 2);//97
	ans.bit[0] = minus;
	return ans;
}

big_number long_long_to_big_number(long long v)
{
	big_number ans;
	memset(ans.bit, 0, sizeof(ans.bit));
	ans.bit[0] = v < 0;
	v = fabs(v);
	for (int i = 255; i > 0; i--)
	{
		ans.bit[i] = v & 1;
		v >>= 1;
	}
	return ans;
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

double_n double_to_double_n(double v)
{
	if (v == 0) return make_double_n(0, 0, 0LL);
	int minus = v < 0;
	v = fabs(v);
	int E = 0;
	if (v > 1)
	{
		while (v >= 2 && E < bias)
			v /= 2, E++;
	}
	else
	{
		while (v < 1 && E > -bias)
			v *= 2, E--;
	}
	if (v >= 2) return make_double_n(minus, (1 << BITS_OF_EXP) - 1, 0LL);
	long long frac = 0;
	v -= 1;
	for (int i = 1; i <= BITS_OF_FRAC; i++)
	{
		v *= 2;
		frac = (frac << 1) + (v >= 1);
		if (v >= 1) v -= 1;
	}
	int exp = E + bias;
	if (exp == 0) frac = (frac >> 1) + (1LL << BITS_OF_FRAC - 1);
	return make_double_n(minus, exp, frac);
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

void print(double_n v)
{
	int exp = exp_of_double_n(v);
	long long frac = frac_of_double_n(v);
	if (exp == (1 << BITS_OF_EXP) - 1)
	{
		(frac == 0) ? printf("inf") : printf("nan");
		return;
	}
	double ans = 0, add = 0.5;
	if (exp != 0) ans = 1;
	for (int i = BITS_OF_FRAC - 1; i >= 0; i--)
	{
		ans += add * ((frac >> i) & 1);
		add /= 2;
	}
	int E = exp - bias;
	while (E > 0)
	{
		ans *= 2;
		E--;
	}
	while (E < 0)
	{
		ans /= 2;
		E++;
	}
	printf("%.50f", ans * ((v.bytes[0] >> 7) == 0 ? 1.0 : -1.0));
}

double_n plus(double_n a,double_n b)
{
	double_n ans;
	int type_a = get_type(a), type_b = get_type(b);
	if (type_a > type_b)
	{
		swap_double_n(&a, &b);
		swap_int(&type_a, &type_b);
	}
	if (type_b == 3) ans = b;
	else if (type_a == 2)
	{
		ans = a;
		if ((a.bytes[0] >> 7) ^ (b.bytes[0] >> 7) == 1) ans.bytes[7]++;
	}
	else if (type_a < 2 && type_b == 2) ans = b;
	else
	{
		int exp_a = exp_of_double_n(a), exp_b = exp_of_double_n(b);
		big_number frac_a = long_long_to_big_number(frac_of_double_n(a)),
				   frac_b = long_long_to_big_number(frac_of_double_n(b));
		if (type_a == 0) frac_a.bit[255 - BITS_OF_FRAC]++;
		else exp_a--;
		if (type_b == 0) frac_b.bit[255 - BITS_OF_FRAC]++;
		else exp_b--;
		if (a.bytes[0] >> 7 == 1) frac_a.bit[0] = 1;
		if (b.bytes[0] >> 7 == 1) frac_b.bit[0] = 1;
		if (exp_a < exp_b)
		{
			swap_int(&exp_a, &exp_b);
			swap_big_number(&frac_a, &frac_b);
		}
		frac_a = shl(frac_a, exp_a - exp_b);
		exp_b = exp_a;
		int base = 1;
		while (frac_a.bit[base] == 0 && base < 255 - BITS_OF_FRAC) base++;
		big_number frac_Ans = plus_big_number(frac_a, frac_b);
		int minus = frac_Ans.bit[0];
		long long frac_ans = 0;
		for (int i = base - 1; i <= base + BITS_OF_FRAC; i++)
			frac_ans = (frac_ans << 1) + frac_Ans.bit[i];
		if (frac_Ans.bit[base + BITS_OF_FRAC + 1] == 1)
		{
			int equal_to_five = 1;
			for (int i = base + BITS_OF_FRAC + 2; i <= 255; i++)
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
		if(exp_ans >= 1 << BITS_OF_EXP - 1)
			ans = make_double_n(minus, (1 << BITS_OF_EXP) - 1, 0LL);
		if (exp_ans == 0) frac_ans >>= 1;
		ans = make_double_n(minus, exp_ans, frac_ans);
	}
	return ans;
}

double_n minus(double_n a,double_n b)
{
	double_n t = b;
	t.bytes[0] ^= 1 << 7;
	return plus(a, t);
}

double_n multiply(double_n a,double_n b)
{
	double_n ans;
	int type_a = get_type(a), type_b = get_type(b);
	if (type_a > type_b)
	{
		swap_double_n(&a, &b);
		swap_int(&type_a, &type_b);
	}
	if (type_b == 3) ans = b;
	else if (type_a == 2)
	{
		ans = a;
		if (ans.bytes[0] >> 7 & 1) ans.bytes[0] -= 1 << 7;
		if ((a.bytes[0] >> 7) ^ (b.bytes[0] >> 7) == 1)
			ans.bytes[0] = ans.bytes[0] + (1 << 7) * ((a.bytes[0] >> 7) ^ (b.bytes[0] >> 7));
	}
	else if (type_a < 2 && type_b == 2) ans = b;
	else
	{
		int exp_a = exp_of_double_n(a), exp_b = exp_of_double_n(b);
		big_number frac_a = long_long_to_big_number(frac_of_double_n(a)),
				   frac_b = long_long_to_big_number(frac_of_double_n(b));
		if (type_a == 0) frac_a.bit[255 - BITS_OF_FRAC]++;
		else
		{
			exp_a--;
			frac_a = shl(frac_a, 1);
		}
		if (type_b == 0) frac_b.bit[255 - BITS_OF_FRAC]++;
		else
		{
			exp_b--;
			frac_b = shl(frac_b, 1);
		}
		big_number frac_Ans = multiply_big_number(frac_a, frac_b);
		int minus = (a.bytes[0] >> 7) ^ (b.bytes[0] >> 7);
		long long frac_ans = 0;
		for (int i = 255 - 2 * BITS_OF_FRAC - 1; i <= 255 - BITS_OF_FRAC; i++)
			frac_ans = (frac_ans << 1) + frac_Ans.bit[i];
		int exp_ans = exp_a + exp_b - bias, dexp = 0;
		while ((frac_ans >> BITS_OF_FRAC + 1 > 1 && exp_ans < (1 << BITS_OF_EXP) - 2) || exp_ans < 0)
		{
			frac_ans = (frac_ans << 1) + frac_Ans.bit[255 - BITS_OF_FRAC + 1 - dexp];
			exp_ans++;
			dexp++;
		}
		while (frac_ans >> BITS_OF_FRAC + 1 == 0 && exp_ans > -1)
		{
			frac_ans <<= 1;
			exp_ans--;
			dexp--;
		}
		if (dexp > 0) frac_Ans = shr(frac_Ans, dexp);
		else frac_Ans = shl(frac_Ans, -dexp);
		if(exp_ans >= 1 << BITS_OF_EXP)
			ans = make_double_n(minus, (1 << BITS_OF_EXP) - 1, 0LL);
		if (exp_ans == 0)
		{
			if (frac_Ans.bit[255 - BITS_OF_FRAC - 1] == 1)
			{
				int equal_to_five = 1;
				for (int i = 255 - BITS_OF_FRAC; i <= 255; i++)
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
			if (frac_Ans.bit[255 - BITS_OF_FRAC] == 1)
			{
				int equal_to_five = 1;
				for (int i = 255 - BITS_OF_FRAC + 1; i <= 255; i++)
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
		ans = make_double_n(minus, exp_ans, frac_ans);
	}
	return ans;
}

double_n divide(double_n a,double_n b)
{
	double_n ans;
	int type_a = get_type(a), type_b = get_type(b);
	if (type_a == 3) return a;
	else if (type_b == 3) return b;
	else if (type_a == 2)
	{
		if (type_b == 2)
		{
			ans = a;
			if (ans.bytes[0] >> 7 & 1) ans.bytes[0] -= 1 << 7;
			if ((a.bytes[0] >> 7) ^ (b.bytes[0] >> 7) == 1)
				ans.bytes[0] = ans.bytes[0] + (1 << 7) * ((a.bytes[0] >> 7) ^ (b.bytes[0] >> 7));
		}
		else if (exp_of_double_n(b) == 0 && frac_of_double_n(b) == 0)
		{
			ans = a;
			ans.bytes[7]++;
		}
	}
	else if (type_a < 2 && type_b == 2)
	{
		memset(ans.bytes, 0, sizeof(ans.bytes));
		ans.bytes[0] += (1 << 7) * ((a.bytes[0] >> 7) & 1);
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
		big_number frac_a = long_long_to_big_number(frac_of_double_n(a)),
				   frac_b = long_long_to_big_number(frac_of_double_n(b));
		if (type_a == 0) frac_a.bit[255 - BITS_OF_FRAC]++;
		else
		{
			exp_a--;
			frac_a = shl(frac_a, 1);
		}
		if (type_b == 0) frac_b.bit[255 - BITS_OF_FRAC]++;
		else
		{
			exp_b--;
			frac_b = shl(frac_b, 1);
		}
		big_number frac_Ans = divide_big_number(frac_a, frac_b);
		int minus = (a.bytes[0] >> 7) ^ (b.bytes[0] >> 7);
		long long frac_ans = 0;
		for (int i = 255 - 2 * BITS_OF_FRAC - 1; i <= 255 - BITS_OF_FRAC; i++)
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
			frac_ans = (frac_ans << 1) + frac_Ans.bit[255 - BITS_OF_FRAC + 1 - dexp];
			exp_ans--;
			dexp--;
		}
		if (dexp > 0) frac_Ans = shr(frac_Ans, dexp);
		else frac_Ans = shl(frac_Ans, -dexp);
		if(exp_ans >= 1 << BITS_OF_EXP)
			ans = make_double_n(minus, (1 << BITS_OF_EXP) - 1, 0LL);
		if (exp_ans == 0)
		{
			if (frac_Ans.bit[255 - BITS_OF_FRAC - 1] == 1)
			{
				int equal_to_five = 1;
				for (int i = 255 - BITS_OF_FRAC; i <= 255; i++)
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
			if (frac_Ans.bit[255 - BITS_OF_FRAC] == 1)
			{
				int equal_to_five = 1;
				for (int i = 255 - BITS_OF_FRAC + 1; i <= 255; i++)
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
		ans = make_double_n(minus, exp_ans, frac_ans);
	}
	return ans;
}

/*void show_bytes_binary_n(double_n v)
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
}*/

int main()
{
	double a, b;
	char type;
	while(scanf("%lf %c %lf", &a, &type, &b) != 0)
	{
		double_n A = double_to_double_n(a), B = double_to_double_n(b);
		double ans;
		double_n ANS;
		if (type == '+')
		{
			ans = a + b;
			ANS = plus(A, B);
		}
		else if (type == '-')
		{
			ans = a - b;
			ANS = minus(A, B);
		}
		else if (type == '*')
		{
			ans = a * b;
			ANS = multiply(A, B);
		}
		else if (type == '/')
		{
			ans = a / b;
			ANS = divide(A, B);
		}
		print(ANS);
		printf("\n");
	}
	return 0;
}
