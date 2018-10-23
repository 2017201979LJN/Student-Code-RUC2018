#include <time.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

#define bool char

#define FALSE 0
#define TRUE 1

#define POS 0
#define NEG 1

#define LARGER 0
#define SMALLER 1
#define EQUAL 2

#define UP 0
#define DOWN 1

#define NUM_OF_BITS_IN_BYTE 8
#define NUM_OF_BYTES_IN_DOUBLE 8
#define NUM_OF_BITS_IN_DOUBLE 64
#define NUM_OF_BITS_IN_LONG_LONG 64
#define NUM_OF_BITS_IN_INT128 128

#define NUM_OF_BITS_OF_EXPO 11
#define NUM_OF_BITS_OF_FRAC 52

#define BIAS 1023

#define EXPO_MIN 0
#define EXPO_MAX 2047
#define EXPO_MAGIC 1993
#define E_MAX 1023
#define E_MIN -1022

#define FRAC_MAX ((1ull << NUM_OF_BITS_OF_FRAC) - 1)

#define POS_OF_INTEGER 104
#define GUARD_BIT 52
#define ROUND_BIT 51
#define STICKY_BIT 50

#define NUM_OF_CASES 10000

#define NUM_OF_EXTREME_NUMBERS 13

FILE *input;
FILE *output_ans, *output_my_ans;

struct my_double
{
	unsigned char bytes[NUM_OF_BYTES_IN_DOUBLE];
};

struct my_int128
{
	unsigned long long fi, se;
};

bool equal(struct my_double, double);

int cmp_abs_my_double(struct my_double, struct my_double);

void show_bytes_double(double);
void show_bits(struct my_double);
void show_bytes(struct my_double);
void show_expo(struct my_double);
void show_E(struct my_double);
void show_frac(struct my_double);

struct my_double gen_rand();
struct my_double gen_zero(bool);
struct my_double gen_inf(bool);
struct my_double gen_NaN(bool);
struct my_double gen_min_denormal(bool);
struct my_double gen_sign_expo_frac(bool, int, unsigned long long);
struct my_double gen_extreme(int);
struct my_double gen_magic(bool);

double trans_to_double(struct my_double);
struct my_double trans_to_my_double(double);

void output_short(FILE*, struct my_double);
void output_long(FILE*, struct my_double);
void output_short_double(FILE*, double);
void output_long_double(FILE*, double);

void swap_int(int*, int*);
void swap_my_double(struct my_double*, struct my_double*);
void swap_my_int128(struct my_int128*, struct my_int128*);

int index_kth_bit(int);
bool kth_bit(struct my_double*, int);
bool sign_bit(struct my_double*);
void set_kth_bit(struct my_double*, int, bool);
void set_sign_bit(struct my_double*, bool);

void set_expo(struct my_double*, int);
void set_frac(struct my_double*, unsigned long long);

int calc_expo(struct my_double);
int calc_E(struct my_double);
unsigned long long calc_frac(struct my_double);

bool is_pos(struct my_double);
bool is_neg(struct my_double);
bool is_normal(struct my_double);
bool is_denormal(struct my_double);
bool is_zero(struct my_double);
bool is_special(struct my_double);
bool is_inf(struct my_double);
bool is_pos_inf(struct my_double);
bool is_neg_inf(struct my_double);
bool is_NaN(struct my_double);

bool is_pos_inf_double(double);
bool is_neg_inf_double(double);
bool is_NaN_double(double);

struct my_double minus_my_double(struct my_double);
struct my_double add_my_double(struct my_double, struct my_double);
struct my_double subtract_my_double(struct my_double, struct my_double);
struct my_double multiply_my_double(struct my_double, struct my_double);
struct my_double divide_my_double(struct my_double, struct my_double);

void show_bits_my_int128(struct my_int128);

int cmp_my_int128(struct my_int128, struct my_int128);

void set_kth_bit_long_long(unsigned long long*, int, bool);
bool kth_bit_my_int128(struct my_int128*, int);
void set_kth_bit_my_int128(struct my_int128*, int, bool);
void add_kth_bit_my_int128(struct my_int128*, int);
void subtract_kth_bit_my_int128(struct my_int128*, int);

struct my_int128 gen_zero_my_int128();

bool is_zero_my_int128(struct my_int128);

struct my_int128 add_my_int128(struct my_int128, struct my_int128);
struct my_int128 subtract_my_int128(struct my_int128, struct my_int128);
struct my_int128 multiply_my_int128(struct my_int128, struct my_int128);
struct my_int128 divide_my_int128(struct my_int128, struct my_int128);

struct my_int128 trans_to_my_int128(struct my_double);
unsigned long long trans_to_frac(struct my_int128);

void left_shift(struct my_int128*);
void left_shift_k_bit(struct my_int128*, int);
void right_shift(struct my_int128*);
void right_shift_k_bit(struct my_int128*, int);
void shift_to_pos_of_integer(struct my_int128*);

int calc_pos_of_first_bit(struct my_int128);

bool rounding_type(struct my_int128);
void rounding(struct my_int128*, int*);

struct my_double adjust(bool, struct my_int128, int);

void init();
void rand_test();
void extreme_test();
void hack();
void test(struct my_double, struct my_double);
void add_test(struct my_double, struct my_double);
void subtract_test(struct my_double, struct my_double);
void multiply_test(struct my_double, struct my_double);
void divide_test(struct my_double, struct my_double);
void output_test(struct my_double, double);
void terminate();

int main()
{
	init();
	rand_test();
	extreme_test();
	hack();
	terminate();
}

bool equal(struct my_double a, double b)
{
	if (is_NaN(a) && is_NaN_double(b)) return TRUE;
	if (is_pos_inf(a) && is_pos_inf_double(b)) return TRUE;
	if (is_neg_inf(a) && is_neg_inf_double(b)) return TRUE;
	unsigned char *pointer = (unsigned char *) &b;
	for (int i = 0; i < NUM_OF_BYTES_IN_DOUBLE; ++i)
	{
		if (pointer[i] != a.bytes[NUM_OF_BYTES_IN_DOUBLE - 1 - i])
		{
			return FALSE;
		}
	}
	return TRUE;
}

int cmp_abs_my_double(struct my_double a, struct my_double b)
{
	for (int i = NUM_OF_BITS_IN_DOUBLE - 2; i >= 0; --i)
	{
		bool bit_a = kth_bit(&a, i);
		bool bit_b = kth_bit(&b, i);
		if (bit_a ^ bit_b)
		{
			if (bit_a) return LARGER;
			return SMALLER;
		}
	}
	return EQUAL;
}

void show_bytes_double(double num)
{
	unsigned char *pointer = (unsigned char *) &num;
	for (int i = NUM_OF_BYTES_IN_DOUBLE - 1; i >= 0; --i)
		printf("%02x ", pointer[i]);
	putchar('\n');
}

void show_bits(struct my_double num)
{
	for (int i = NUM_OF_BITS_IN_DOUBLE - 1; i >= 0; --i)
		printf("%d", (int) (kth_bit(&num, i)));
	putchar('\n');
}

void show_bytes(struct my_double num)
{
	for (int i = 0; i < NUM_OF_BYTES_IN_DOUBLE; ++i)
		printf("%02x ", num.bytes[i]);
	putchar('\n');
}

void show_expo(struct my_double num)
{
	printf("%d\n", calc_expo(num));
}

void show_E(struct my_double num)
{
	printf("%d\n", calc_E(num));
}

void show_frac(struct my_double num)
{
	printf("%llu\n", calc_frac(num));
}

struct my_double gen_rand()
{
	struct my_double res;
	for (int i = 0; i < NUM_OF_BYTES_IN_DOUBLE; ++i)
		res.bytes[i] = (unsigned char) (rand() % (UCHAR_MAX+1));
	return res;
}

struct my_double gen_zero(bool sign)
{
	struct my_double res;
	for (int i = 0; i < NUM_OF_BYTES_IN_DOUBLE; ++i)
		res.bytes[i] = 0;
	set_sign_bit(&res, sign);
	return res;
}

struct my_double gen_inf(bool sign)
{
	struct my_double res;
	set_sign_bit(&res, sign);
	set_expo(&res, EXPO_MAX);
	set_frac(&res, 0ull);
	return res;
}

struct my_double gen_NaN(bool sign)
{
	struct my_double res;
	set_sign_bit(&res, sign);
	set_expo(&res, EXPO_MAX);
	set_frac(&res, 1ull << (NUM_OF_BITS_OF_FRAC - 1));
	return res;
}

struct my_double gen_min_denormal(bool sign)
{
	return gen_sign_expo_frac(sign, EXPO_MIN, 1ull);
}

struct my_double gen_max_normal(bool sign)
{
	return gen_sign_expo_frac(sign, EXPO_MAX - 1, FRAC_MAX);
}

struct my_double gen_sign_expo_frac(bool sign, int expo, unsigned long long frac)
{
	struct my_double res;
	set_sign_bit(&res, sign);
	set_expo(&res, expo);
	set_frac(&res, frac);
	return res;
}

struct my_double gen_extreme(int id)
{
	switch (id)
	{
		case 0:
			return gen_rand();
		case 1:
			return gen_zero(POS);
		case 2:
			return gen_zero(NEG);
		case 3:
			return gen_inf(POS);
		case 4:
			return gen_inf(NEG);
		case 5:
			return gen_min_denormal(POS);
		case 6:
			return gen_min_denormal(NEG);
		case 7:
			return gen_NaN(POS);
		case 8:
			return gen_NaN(NEG);
		case 9:
			return gen_max_normal(POS);
		case 10:
			return gen_max_normal(NEG);
		case 11:
			return gen_magic(POS);
		case 12:
			return gen_magic(NEG);
	}
}

struct my_double gen_magic(bool sign)
{
	return gen_sign_expo_frac(sign, EXPO_MAGIC, 0ull);
}

double trans_to_double(struct my_double num)
{
	double res;
	unsigned char* pointer = (unsigned char*) &res;
	for (int i = NUM_OF_BYTES_IN_DOUBLE - 1; i >= 0; --i, ++pointer)
		*pointer = num.bytes[i];
	return res;
}

struct my_double trans_to_my_double(double num)
{
	struct my_double res;
	unsigned char* pointer = (unsigned char*) &num;
	for (int i = NUM_OF_BYTES_IN_DOUBLE - 1; i >= 0; --i, ++pointer)
		res.bytes[i] = *pointer;
	return res;
}

void output_short(FILE* output, struct my_double num)
{
	output_short_double(output, trans_to_double(num));
}

void output_long(FILE* output, struct my_double num)
{
	output_long_double(output, trans_to_double(num));
}

void output_short_double(FILE* output, double num)
{
	fprintf(output, "%le\n", num);
}

void output_long_double(FILE* output, double num)
{
	fprintf(output, "%.50le\n", num);
}

void swap_int(int* a, int* b)
{
	int c = *a;
	*a = *b;
	*b = c;
}

void swap_my_double(struct my_double* a, struct my_double* b)
{
	struct my_double c = *a;
	*a = *b;
	*b = c;
}

void swap_my_int128(struct my_int128* a, struct my_int128* b)
{
	struct my_int128 c = *a;
	*a = *b;
	*b = c;
}

int index_kth_bit(int k)
{
	return NUM_OF_BYTES_IN_DOUBLE - 1 - (k / NUM_OF_BITS_IN_BYTE);
}

bool kth_bit(struct my_double* num, int k)
{
	int index = index_kth_bit(k);
	k %= NUM_OF_BITS_IN_BYTE;
	return (bool) (((*num).bytes[index] >> k) & 1);
}

bool sign_bit(struct my_double* num)
{
	return kth_bit(num, NUM_OF_BITS_IN_DOUBLE - 1);
}

void set_kth_bit(struct my_double* num, int k, bool bit)
{
	int index = index_kth_bit(k);
	k %= NUM_OF_BITS_IN_BYTE;
	(*num).bytes[index] &= UCHAR_MAX ^ (1 << k);
	(*num).bytes[index] |= ((unsigned char) (bit)) << k;
}

void set_sign_bit(struct my_double* num, bool bit)
{
	set_kth_bit(num, NUM_OF_BITS_IN_DOUBLE - 1, bit);
}

void set_expo(struct my_double* num, int expo)
{
	for (int i = NUM_OF_BITS_OF_FRAC; i < NUM_OF_BITS_IN_DOUBLE - 1; ++i, expo >>= 1)
	{
		set_kth_bit(num, i, (bool) (expo & 1));
	}
}

void set_frac(struct my_double* num, unsigned long long frac)
{
	for (int i = 0; i < NUM_OF_BITS_OF_FRAC; ++i, frac >>= 1)
	{
		set_kth_bit(num, i, (bool) (frac & 1));
	}
}

int calc_expo(struct my_double num)
{
	int res = 0;
	for (int i = NUM_OF_BITS_OF_FRAC, bit = 0; i < NUM_OF_BITS_IN_DOUBLE - 1; ++i, ++bit)
	{
		res |= ((unsigned char) (kth_bit(&num, i))) << bit;
	}
	return res;
}

int calc_E(struct my_double num)
{
	int expo = calc_expo(num);
	if (expo > EXPO_MIN) return expo - BIAS;
	return 1 - BIAS;
}

unsigned long long calc_frac(struct my_double num)
{
	unsigned long long res = 0;
	for (int i = 0; i < NUM_OF_BITS_OF_FRAC; ++i)
		res |= ((unsigned long long) kth_bit(&num, i)) << i;
	return res;
}

bool is_pos(struct my_double num)
{
	return sign_bit(&num) == POS;
}

bool is_neg(struct my_double num)
{
	return sign_bit(&num) == NEG;
}

bool is_normal(struct my_double num)
{
	int expo = calc_expo(num);
	return expo > EXPO_MIN && expo < EXPO_MAX;
}

bool is_denormal(struct my_double num)
{
	return calc_expo(num) == EXPO_MIN;
}

bool is_zero(struct my_double num)
{
	return is_denormal(num) && calc_frac(num) == 0ull;
}

bool is_special(struct my_double num)
{
	return calc_expo(num) == EXPO_MAX;
}

bool is_inf(struct my_double num)
{
	return is_special(num) && calc_frac(num) == 0ull;
}

bool is_pos_inf(struct my_double num)
{
	return is_pos(num) && is_inf(num);
}

bool is_neg_inf(struct my_double num)
{
	return is_neg(num) && is_inf(num);
}

bool is_NaN(struct my_double num)
{
	return is_special(num) && calc_frac(num) > 0ull;
}

bool is_pos_inf_double(double num)
{
	return is_pos_inf(trans_to_my_double(num));
}

bool is_neg_inf_double(double num)
{
	return is_neg_inf(trans_to_my_double(num));
}

bool is_NaN_double(double num)
{
	return is_NaN(trans_to_my_double(num));
}

struct my_double minus_my_double(struct my_double num)
{
	struct my_double res = num;
	set_sign_bit(&res, sign_bit(&num) ^ 1);
	return res;
}

struct my_double add_my_double(struct my_double a, struct my_double b)
{
	if (is_NaN(a)) return a;
	if (is_NaN(b)) return b;
	if (is_inf(a) && is_inf(b))
	{
		if (sign_bit(&a) ^ sign_bit(&b)) return gen_NaN(NEG);
		return a;
	}
	if (is_inf(a)) return a;
	if (is_inf(b)) return b;

	// now a, b are normalized or denormalized
	int E1 = calc_E(a), E2 = calc_E(b);
	if (E1 < E2)
	{
		swap_my_double(&a, &b);
		swap_int(&E1, &E2);
	}
	struct my_int128 M1 = trans_to_my_int128(a), M2 = trans_to_my_int128(b);
	shift_to_pos_of_integer(&M1);
	shift_to_pos_of_integer(&M2);
	right_shift_k_bit(&M2, E1 - E2);
	int E = E1;
	struct my_int128 M;
	bool sign;
	if (sign_bit(&a) == sign_bit(&b))
	{
		sign = sign_bit(&a);
		M = add_my_int128(M1, M2);
	}
	else
	{
		if (cmp_abs_my_double(a, b) == SMALLER)
		{	
			swap_my_double(&a, &b);
			swap_my_int128(&M1, &M2);
		}
		if (cmp_abs_my_double(a, b) == EQUAL) sign = POS;
		else sign = sign_bit(&a);
		M = subtract_my_int128(M1, M2);
	}
	return adjust(sign, M, E);
}

struct my_double subtract_my_double(struct my_double a, struct my_double b)
{
	if (is_NaN(a)) return a;
	if (is_NaN(b)) return b;
	return add_my_double(a, minus_my_double(b));
}

struct my_double multiply_my_double(struct my_double a, struct my_double b)
{
	if (is_NaN(a)) return a;
	if (is_NaN(b)) return b;
	bool sign = sign_bit(&a) ^ sign_bit(&b);
	if (is_inf(a) && is_inf(b)) return gen_inf(sign);
	if (!is_inf(a) &&  is_inf(b)) swap_my_double(&a, &b);
	if ( is_inf(a) && !is_inf(b))
	{
		if (is_zero(b)) return gen_NaN(sign);
		return gen_inf(sign);
	}

	// now a, b are normalized or denormalized
	int E1 = calc_E(a), E2 = calc_E(b);
	int E = E1 + E2;
	struct my_int128 M1, M2, M;
	M1 = trans_to_my_int128(a);
	M2 = trans_to_my_int128(b);
	M = multiply_my_int128(M1, M2);
	return adjust(sign, M, E);
}

struct my_double divide_my_double(struct my_double a, struct my_double b)
{
	if (is_NaN(a)) return a;
	if (is_NaN(b)) return b;
	bool sign = sign_bit(&a) ^ sign_bit(&b);
	if (is_inf(a))
	{
		if (is_inf(b)) return gen_NaN(NEG);
		return gen_inf(sign);
	}
	if (is_inf(b)) return gen_zero(sign);
	if (is_zero(b))
	{
		if(is_zero(a)) return gen_NaN(NEG);
		return gen_inf(sign);
	}

	// now a, b are normalized or denormalized
	int E1 = calc_E(a), E2 = calc_E(b);
	struct my_int128 M1, M2, M;
	M1 = trans_to_my_int128(a);
	M2 = trans_to_my_int128(b);
	if (is_denormal(a))
	{
		int k = NUM_OF_BITS_OF_FRAC - calc_pos_of_first_bit(M1);
		left_shift_k_bit(&M1, k);
		E1 -= k;
	}
	if (is_denormal(b))
	{
		int k = NUM_OF_BITS_OF_FRAC - calc_pos_of_first_bit(M2);
		left_shift_k_bit(&M2, k);
		E2 -= k;
	}
	int E = E1 - E2;
	shift_to_pos_of_integer(&M1);
	shift_to_pos_of_integer(&M2);
	M = divide_my_int128(M1, M2);
	return adjust(sign, M, E);
}

void show_bits_my_int128(struct my_int128 num)
{
	for (int i = NUM_OF_BITS_IN_INT128 - 1; i >= 0; --i)
		printf("%d", (int)(kth_bit_my_int128(&num, i)));
	putchar('\n');
}

int cmp_my_int128(struct my_int128 a, struct my_int128 b)
{
	for (int i = NUM_OF_BITS_IN_INT128 - 1; i >= 0; --i)
	{
		bool bit_a = kth_bit_my_int128(&a, i);
		bool bit_b = kth_bit_my_int128(&b, i);
		if (bit_a == 1 && bit_b == 0) return LARGER;
		if (bit_a == 0 && bit_b == 1) return SMALLER;
	}
	return EQUAL;
}

bool kth_bit_my_int128(struct my_int128* num, int k)
{
	if (k < NUM_OF_BITS_IN_LONG_LONG) return (bool) (((*num).se >> k) & 1);
	k -= NUM_OF_BITS_IN_LONG_LONG;
	return (bool) (((*num).fi >> k) & 1);
}

void set_kth_bit_long_long(unsigned long long* num, int k, bool bit)
{
	*num &= ULLONG_MAX ^ (1ull << k);
	*num |= ((unsigned long long) (bit)) << k;
}

void set_kth_bit_my_int128(struct my_int128* num, int k, bool bit)
{
	if (k < NUM_OF_BITS_IN_LONG_LONG) return set_kth_bit_long_long(&((*num).se), k, bit);
	k -= NUM_OF_BITS_IN_LONG_LONG;
	set_kth_bit_long_long(&((*num).fi), k, bit);
}

void add_kth_bit_my_int128(struct my_int128* num, int k)
{
	for (; k < NUM_OF_BITS_IN_INT128 && kth_bit_my_int128(num, k) == 1; ++k)
	{
		set_kth_bit_my_int128(num, k, 0);
	}
	if (k < NUM_OF_BITS_IN_INT128) set_kth_bit_my_int128(num, k, 1);
}

void subtract_kth_bit_my_int128(struct my_int128* num, int k)
{
	for (; kth_bit_my_int128(num, k) == 0; ++k)
	{
		set_kth_bit_my_int128(num, k, 1);
	}
	set_kth_bit_my_int128(num, k, 0);
}

struct my_int128 gen_zero_my_int128()
{
	struct my_int128 res;
	res.fi = res.se = 0ull;
	return res;
}

bool is_zero_my_int128(struct my_int128 num)
{
	return num.fi == 0ull && num.se == 0ull;
}

struct my_int128 add_my_int128(struct my_int128 a, struct my_int128 b)
{
	struct my_int128 res;
	res.fi = a.fi + b.fi;
	res.se = a.se + b.se;
	if (res.se < a.se) ++res.fi;
	return res;
}

struct my_int128 subtract_my_int128(struct my_int128 a, struct my_int128 b)
{
	struct my_int128 res;
	res.fi = a.fi - b.fi;
	res.se = a.se - b.se;
	if (res.se > a.se) --res.fi;
	return res;
}

struct my_int128 multiply_my_int128(struct my_int128 a, struct my_int128 b)
{
	struct my_int128 res;
	res = gen_zero_my_int128();
	for (int i = 0; i < NUM_OF_BITS_IN_LONG_LONG; ++i)
	{
		if (kth_bit_my_int128(&a, i) == 1)
		{
			for (int j = 0; j < NUM_OF_BITS_IN_LONG_LONG; ++j)
			{
				if (kth_bit_my_int128(&b, j) == 1)
				{
					add_kth_bit_my_int128(&res, i + j);
				}
			}
		}
	}
	return res;
}

struct my_int128 divide_my_int128(struct my_int128 a, struct my_int128 b)
{
	struct my_int128 res;
	res = gen_zero_my_int128();
	for (int i = POS_OF_INTEGER; i >= 0; --i, right_shift(&b))
	{
		if (cmp_my_int128(b, a) != LARGER)
		{
			set_kth_bit_my_int128(&res, i, 1);
			a = subtract_my_int128(a ,b);
		}
	}
	return res;
}

struct my_int128 trans_to_my_int128(struct my_double num)
{
	struct my_int128 res;
	res.fi = 0ull;
	res.se = calc_frac(num);
	if (is_normal(num)) res.se |= 1ull << NUM_OF_BITS_OF_FRAC;
	return res;
}

unsigned long long trans_to_frac(struct my_int128 num)
{
	unsigned long long res = 0ull;
	for (int i = POS_OF_INTEGER - NUM_OF_BITS_OF_FRAC, k = 0; i < POS_OF_INTEGER; ++i, ++k)
	{
		bool bit = kth_bit_my_int128(&num, i);
		res |= ((unsigned long long) (bit)) << k;
	}
	return res;
}

void left_shift(struct my_int128* M)
{
	left_shift_k_bit(M, 1);
}

void left_shift_k_bit(struct my_int128* M, int k)
{
	for (int i = NUM_OF_BITS_IN_INT128 - 1; i >= 0; --i)
	{
		bool bit = 0;
		if (i - k >= 0) bit = kth_bit_my_int128(M, i - k);
		set_kth_bit_my_int128(M, i, bit);
	}
}

void right_shift(struct my_int128* M)
{
	right_shift_k_bit(M, 1);
}

void right_shift_k_bit(struct my_int128* M, int k)
{
	for (int i = 0; i < NUM_OF_BITS_IN_INT128; ++i)
	{
		bool bit = 0;
		if (i + k < NUM_OF_BITS_IN_INT128) bit = kth_bit_my_int128(M, i + k);
		set_kth_bit_my_int128(M, i, bit);
	}
}

void shift_to_pos_of_integer(struct my_int128* M)
{
	left_shift_k_bit(M, POS_OF_INTEGER - NUM_OF_BITS_OF_FRAC);
}

int calc_pos_of_first_bit(struct my_int128 num)
{
	for (int i = NUM_OF_BITS_IN_INT128 - 1; i >= 0; --i)
	{
		if (kth_bit_my_int128(&num, i) == 1)
		{
			return i;
		}
	}
	return -1;
}

bool rounding_type(struct my_int128 num)
{
	if (kth_bit_my_int128(&num, ROUND_BIT) == 0) return DOWN;
	for (int i = STICKY_BIT; i >= 0; --i)
	{
		if (kth_bit_my_int128(&num, i) == 1)
		{
			return UP;
		}
	}
	if (kth_bit_my_int128(&num, GUARD_BIT) == 1) return UP;
	return DOWN;
}

void rounding(struct my_int128* M, int* E)
{
	bool type = rounding_type(*M);
	if (type == UP)
	{
		add_kth_bit_my_int128(M, GUARD_BIT);
		if (kth_bit_my_int128(M, POS_OF_INTEGER + 1) == 1)
		{
			right_shift(M);
			++(*E);
			if(*E > E_MAX)
			{
				(*M) = gen_zero_my_int128();
			}
		}
	}
}

struct my_double adjust(bool sign, struct my_int128 M, int E)
{
	int pos_of_first_bit = calc_pos_of_first_bit(M);
	if (pos_of_first_bit == -1) return gen_zero(sign);
	if (pos_of_first_bit > POS_OF_INTEGER)
	{
		int delta = pos_of_first_bit - POS_OF_INTEGER;
		right_shift_k_bit(&M, delta);
		E += delta;
	}
	else if (pos_of_first_bit < POS_OF_INTEGER)
	{
		int delta = POS_OF_INTEGER - pos_of_first_bit;
		left_shift_k_bit(&M, delta);
		E -= delta;
	}
	if (E > E_MAX) return gen_inf(sign);
	if (E < E_MIN) // denormalized
	{
		right_shift_k_bit(&M, E_MIN - E);
		E = E_MIN;
	}
	rounding(&M, &E);
	if (E > E_MAX)
	{
		return gen_inf(sign);
	}
	int expo;
	if (kth_bit_my_int128(&M, POS_OF_INTEGER) == 1) expo = E + BIAS;
	else expo = EXPO_MIN;
	unsigned long long frac = trans_to_frac(M);
	return gen_sign_expo_frac(sign, expo, frac);
}

void init()
{
	input = fopen("data.in", "r");
	output_ans = fopen("ans.out", "w");
	output_my_ans = fopen("my_ans.out", "w");
	srand(time(NULL));
}

void rand_test()
{
	for (int i = 0; i < NUM_OF_CASES; ++i)
	{
		struct my_double a, b;
		a = gen_rand();
		b = gen_rand();
		test(a, b);
	}
}

void extreme_test()
{
	for (int i = 0; i < NUM_OF_EXTREME_NUMBERS; ++i)
		for (int j = 0; j < NUM_OF_EXTREME_NUMBERS; ++j)
		{
			test(gen_extreme(i), gen_extreme(j));
		}
}

void hack()
{
	double a, b;
	while (fscanf(input, "%lf %lf", &a, &b) != EOF)
	{
		struct my_double my_a, my_b;
		my_a = trans_to_my_double(a);
		my_b = trans_to_my_double(b);
		test(my_a, my_b);
	}
}

void test(struct my_double a, struct my_double b)
{
	fprintf(output_ans, "a: ");
	fprintf(output_my_ans, "a: ");
	output_long(output_ans, a);
	output_long(output_my_ans, a);
	fprintf(output_ans, "b: ");
	fprintf(output_my_ans, "b: ");
	output_long(output_ans, b);
	output_long(output_my_ans, b);

	add_test(a, b);
	subtract_test(a, b);
	multiply_test(a, b);
	divide_test(a, b);

	fprintf(output_ans, "\n");
	fprintf(output_my_ans, "\n");
}

void add_test(struct my_double a, struct my_double b)
{
	fprintf(output_ans, "+:\n");
	fprintf(output_my_ans, "+:\n");
	struct my_double res = add_my_double(a, b);
	double real_res = trans_to_double(a) + trans_to_double(b);
	output_test(res, real_res);
}

void subtract_test(struct my_double a, struct my_double b)
{
	fprintf(output_ans, "-:\n");
	fprintf(output_my_ans, "-:\n");
	struct my_double res = subtract_my_double(a, b);
	double real_res = trans_to_double(a) - trans_to_double(b);
	output_test(res, real_res);
}

void multiply_test(struct my_double a, struct my_double b)
{
	fprintf(output_ans, "*:\n");
	fprintf(output_my_ans, "*:\n");
	struct my_double res = multiply_my_double(a, b);
	double real_res = trans_to_double(a) * trans_to_double(b);
	output_test(res, real_res);
}

void divide_test(struct my_double a, struct my_double b)
{
	fprintf(output_ans, "/:\n");
	fprintf(output_my_ans, "/:\n");
	struct my_double res = divide_my_double(a, b);
	double real_res = trans_to_double(a) / trans_to_double(b);
	output_test(res, real_res);
}

void output_test(struct my_double a, double b)
{
	output_short(output_my_ans, a);
	output_short_double(output_ans, b);

	output_long(output_my_ans, a);
	output_long_double(output_ans, b);
}

void terminate()
{
	fclose(input);
	fclose(output_ans);
	fclose(output_my_ans);
}

