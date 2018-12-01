#include <time.h>
#include <math.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

#define bool char

#define FALSE 0
#define TRUE  1

#define POS 0
#define NEG 1

// for comparison of numbers
#define LARGER  0
#define SMALLER 1
#define EQUAL   2

// for rounding
#define UP   0
#define DOWN 1

#define NUM_OF_BITS_IN_BYTE 8

#define NUM_OF_BYTES_IN_DOUBLE 8
#define NUM_OF_BITS_IN_DOUBLE 64
#define NUM_OF_BITS_IN_LONG_LONG 64
#define NUM_OF_BITS_IN_INT128 128

#define NUM_OF_BITS_OF_SIGN 1
#define NUM_OF_BITS_OF_EXPO 11
#define NUM_OF_BITS_OF_FRAC 52

#define BIAS 1023

#define EXPO_MIN 0
#define EXPO_MAX 2047
#define EXPO_MAGIC 1993
#define E_MAX 1023
#define E_MIN -1022

#define FRAC_MAX ((1uLL << NUM_OF_BITS_OF_FRAC) - 1)

// for my_int128
#define POS_OF_INTEGER_IN_MY_INT128 104
#define GUARD_BIT 52
#define ROUND_BIT 51
#define STICKY_BIT 50

#define NUM_OF_EXTREME_NUMBERS 13

// for big_demical
#define NUM_OF_DIGITS_IN_BIG_NUMBER 2500
#define POS_OF_INTEGER_IN_BIG_NUMBER 1050

// characters
#define ADD '+'
#define SUBTRACT '-'
#define MULTIPLY '*'
#define DIVIDE '/'
#define TAB '\t'
#define SPACE ' '
#define NEWLINE '\n'
#define LEFT_PARENTHESIS  '('
#define RIGHT_PARENTHESIS ')'
#define DECIMAL_POINT '.'
#define NULL_CHARACTER '\0'
#define ZERO '0'
#define LOWER_E 'e'
#define UPPER_E 'E'
#define TERMINATOR '$'

#define POS_INF "inf"
#define NEG_INF "-inf"
#define POS_NAN "nan"
#define NEG_NAN "-nan"

// bases
#define BASE_DECIMAL 10
#define BASE_BINARY  2

#define LEN_OF_STR 1000
#define BUF_SIZE 1000

#define NUMBER '0' // signal that a number was found

#define NOT_FOUND -1

#define NUM_OF_DIGITS_OF_EXPO 2
#define PRECISION 50

#define CARRY_LIMIT 5

// for input and output
#define READ_ONLY "r"
#define WRITE_ONLY "w"
#define INPUT "data.in"
#define OUTPUT "data.out"

struct my_double
{
	unsigned char bytes[NUM_OF_BYTES_IN_DOUBLE];
};

struct my_int128
{
	unsigned long long fi, se;
};

struct big_number
{
	int digits[NUM_OF_DIGITS_IN_BIG_NUMBER];
};

int cmp_abs__my_double(struct my_double, struct my_double);

void output_expo(int);
void output__my_double(struct my_double);

struct my_double gen_rand();
struct my_double gen_zero(bool);
struct my_double gen_inf(bool);
struct my_double gen_NaN(bool);
struct my_double gen_non_special_rand();
struct my_double gen_sign_expo_frac(bool, int, unsigned long long);

struct my_double string_to_my_double(char []);

void swap__int(int*, int*);
void swap__my_double(struct my_double*, struct my_double*);
void swap__my_int128(struct my_int128*, struct my_int128*);

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

struct my_double minus__my_double(struct my_double);
struct my_double add__my_double(struct my_double, struct my_double);
struct my_double subtract__my_double(struct my_double, struct my_double);
struct my_double multiply__my_double(struct my_double, struct my_double);
struct my_double divide__my_double(struct my_double, struct my_double);

struct my_double calculate__my_double(struct my_double, char, struct my_double);

int cmp__my_int128(struct my_int128, struct my_int128);

void set_kth_bit_long_long(unsigned long long*, int, bool);
bool kth_bit__my_int128(struct my_int128*, int);
void set_kth_bit__my_int128(struct my_int128*, int, bool);
void add_kth_bit__my_int128(struct my_int128*, int);
void subtract_kth_bit__my_int128(struct my_int128*, int);

struct my_int128 gen_zero__my_int128();

bool is_zero__my_int128(struct my_int128);

struct my_int128 add__my_int128(struct my_int128, struct my_int128);
struct my_int128 subtract__my_int128(struct my_int128, struct my_int128);
struct my_int128 multiply__my_int128(struct my_int128, struct my_int128);
struct my_int128 divide__my_int128(struct my_int128, struct my_int128);

struct my_int128 my_double_to_my_int128(struct my_double);
unsigned long long my_int128_to_frac(struct my_int128);

void left_shift(struct my_int128*);
void left_shift_k_bit(struct my_int128*, int);
void right_shift(struct my_int128*);
void right_shift_k_bit(struct my_int128*, int);
void shift_to_pos_of_integer(struct my_int128*);

int calc_pos_of_first_bit(struct my_int128);

bool rounding_type(struct my_int128);
void rounding(struct my_int128*, int*);

struct my_double adjust(bool, struct my_int128, int);

int char_to_value(int);
int value_to_char(int);
int getch(int [], int*);
void ungetch(int [], int*, int);

// evaluation of expressions
struct my_double evaluate(int [], int*);
int get_ope(int [], int*, char []);
void push_operand(struct my_double [], int*, struct my_double);
void push_operator(int [], int*, int, struct my_double [], int*);
void calculate_top_stk(int [], int*, struct my_double [], int*);
int priority(int);

// operations of big_number
void set_zero__big_number(struct big_number*);
void set_one__big_number(struct big_number*);
bool has_integer_part__big_number(struct big_number*);
void left_shift_k_digits__big_number(struct big_number*, int);
void right_shift_k_digits__big_number(struct big_number*, int);
void left_shift__big_number(struct big_number*);
void right_shift__big_number(struct big_number*);

void set_kth_digit__big_number(struct big_number*, int, int);

void integer_part_divide_by_two__big_number(struct big_number*);
bool decimal_part_multiply_by_two__big_number(struct big_number*);
void multiply_by_two__big_number(struct big_number*);
void divide_by_two__big_number(struct big_number*);
void add__big_number(struct big_number*, struct big_number*);

int calc_pos_of_first_digit__big_number(struct big_number*);

void init();
void process();


int main()
{
	init();
	process();
}

int cmp_abs__my_double(struct my_double a, struct my_double b)
{
	for (int i = NUM_OF_BITS_IN_DOUBLE - NUM_OF_BITS_OF_SIGN - 1; i >= 0; --i)
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

void output_expo(int expo)
{
	putchar(LOWER_E);
	if (expo >= 0) putchar(ADD);
	else
	{
		putchar(SUBTRACT);
		expo = -expo;
	}
	if (expo == 0)
	{
		for (int i = 0; i < NUM_OF_DIGITS_OF_EXPO; ++i)
			putchar(ZERO);
		return;
	}
	int num_of_digits = 0;
	for (int tmp = expo; tmp > 0; tmp /= BASE_DECIMAL, ++num_of_digits)
		;
	for (int i = 0; i < NUM_OF_DIGITS_OF_EXPO - num_of_digits; ++i)
		putchar(ZERO);
	printf("%d", expo);
}

void output__my_double(struct my_double num)
{
	if (is_pos_inf(num))
	{
		printf(POS_INF);
		return;
	}
	if (is_neg_inf(num))
	{
		printf(NEG_INF);
		return;
	}
	if (is_NaN(num))
	{
		printf(NEG_NAN);
		return;
	}
	static struct big_number decimal;
	static struct big_number power;
	if (is_normal(num)) set_one__big_number(&decimal);
	else set_zero__big_number(&decimal);
	set_one__big_number(&power);
	for (int i = NUM_OF_BITS_OF_FRAC - 1; i >= 0; --i)
	{
		divide_by_two__big_number(&power);
		if (kth_bit(&num, i) == 1)
		{
			add__big_number(&decimal, &power);
		}
	}
	int E = calc_E(num);
	if (E > 0)
	{
		for (int i = 0; i < E; ++i)
		{
			multiply_by_two__big_number(&decimal);
		}
	}
	else if (E < 0)
	{
		for (int i = 0; i < -E; ++i)
		{
			divide_by_two__big_number(&decimal);
		}
	}

	if (is_neg(num)) putchar(SUBTRACT);
	int pos_of_first_digit = calc_pos_of_first_digit__big_number(&decimal);
	if (pos_of_first_digit == NOT_FOUND)
	{
		putchar(ZERO);
		putchar(DECIMAL_POINT);
		for (int i = 0; i < PRECISION; ++i)
			putchar(ZERO);
		output_expo(0);
	}
	else
	{
		// rounding
		if (pos_of_first_digit + PRECISION + 1 < NUM_OF_DIGITS_IN_BIG_NUMBER && decimal.digits[pos_of_first_digit + PRECISION + 1] >= CARRY_LIMIT)
		{
			for (int i = pos_of_first_digit + PRECISION; ; --i)
			{
				++decimal.digits[i];
				if(decimal.digits[i] == BASE_DECIMAL)
				{
					decimal.digits[i] = 0;
				}
				else break;
			}
			if (decimal.digits[pos_of_first_digit - 1] > 0)
				--pos_of_first_digit;
		}
		putchar(value_to_char(decimal.digits[pos_of_first_digit]));
		putchar(DECIMAL_POINT);
		for (int i = 0, pos = pos_of_first_digit + 1; i < PRECISION; ++i, ++pos)
		{
			if (pos < NUM_OF_DIGITS_IN_BIG_NUMBER) putchar(value_to_char(decimal.digits[pos]));
			else putchar(ZERO);
		}
		output_expo(POS_OF_INTEGER_IN_BIG_NUMBER - pos_of_first_digit);
	}
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
	set_frac(&res, 0uLL);
	return res;
}

struct my_double gen_NaN(bool sign)
{
	struct my_double res;
	set_sign_bit(&res, sign);
	set_expo(&res, EXPO_MAX);
	set_frac(&res, 1uLL << (NUM_OF_BITS_OF_FRAC - 1));
	return res;
}

struct my_double gen_non_special_rand()
{
	struct my_double res;
	do
	{
		res = gen_rand();
	}
	while (is_special(res));
	return res;
}

struct my_double gen_sign_expo_frac(bool sign, int expo, unsigned long long frac)
{
	struct my_double res;
	set_sign_bit(&res, sign);
	set_expo(&res, expo);
	set_frac(&res, frac);
	return res;
}

struct my_double string_to_my_double(char str[])
{
	int i = 0, exp;
	bool sign, exp_sign;
	sign = str[i] == SUBTRACT ? NEG : POS;
	if (str[i] == ADD || str[i] == SUBTRACT) ++i;
	
	static struct big_number decimal;
	set_zero__big_number(&decimal);
	for (; isdigit(str[i]); ++i)
	{
		left_shift__big_number(&decimal);
		set_kth_digit__big_number(&decimal, POS_OF_INTEGER_IN_BIG_NUMBER, char_to_value(str[i]));
	}

	if (str[i] == DECIMAL_POINT) ++i;
	for (int pos = POS_OF_INTEGER_IN_BIG_NUMBER + 1; isdigit(str[i]); ++i, ++pos)
		set_kth_digit__big_number(&decimal, pos, char_to_value(str[i]));
	
	if (str[i] == LOWER_E || str[i] == UPPER_E) ++i;
	exp_sign = str[i] == SUBTRACT ? NEG : POS;
	if (str[i] == ADD || str[i] == SUBTRACT) ++i;
	exp = 0;
	for (; isdigit(str[i]); ++i)
		exp = exp * BASE_DECIMAL + char_to_value(str[i]);
	if (exp_sign == POS) 
	{
		left_shift_k_digits__big_number(&decimal, exp);
	}
	else
	{
		right_shift_k_digits__big_number(&decimal, exp);
	}
	
	static struct big_number binary;
	set_zero__big_number(&binary);
	for (int pos = POS_OF_INTEGER_IN_BIG_NUMBER; has_integer_part__big_number(&decimal); --pos)
	{
		set_kth_digit__big_number(&binary, pos, decimal.digits[POS_OF_INTEGER_IN_BIG_NUMBER] & 1);
		integer_part_divide_by_two__big_number(&decimal);
	}
	for (int pos = POS_OF_INTEGER_IN_BIG_NUMBER + 1; pos < NUM_OF_DIGITS_IN_BIG_NUMBER; ++pos)
	{
		if (decimal_part_multiply_by_two__big_number(&decimal))
		{
			set_kth_digit__big_number(&binary, pos, 1);
		}
	}
	
	struct my_int128 M;
	int E = 0;
	M = gen_zero__my_int128();
	int pos_of_first_digit = calc_pos_of_first_digit__big_number(&binary);
	if (pos_of_first_digit != NOT_FOUND)
	{
		for (int i = POS_OF_INTEGER_IN_MY_INT128, j = pos_of_first_digit; i >= 0 && j < NUM_OF_DIGITS_IN_BIG_NUMBER; --i, ++j)
		{
			set_kth_bit__my_int128(&M, i, binary.digits[j]);
		}
		E = POS_OF_INTEGER_IN_BIG_NUMBER - pos_of_first_digit;
	}
	return adjust(sign, M, E);
}

double my_double_to_double(struct my_double num)
{
	double res;
	unsigned char* pointer = (unsigned char*) &res;
	for (int i = NUM_OF_BYTES_IN_DOUBLE - 1; i >= 0; --i, ++pointer)
		*pointer = num.bytes[i];
	return res;
}

void swap__int(int* a, int* b)
{
	int c = *a;
	*a = *b;
	*b = c;
}

void swap__my_double(struct my_double* a, struct my_double* b)
{
	struct my_double c = *a;
	*a = *b;
	*b = c;
}

void swap__my_int128(struct my_int128* a, struct my_int128* b)
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
	return is_denormal(num) && calc_frac(num) == 0uLL;
}

bool is_special(struct my_double num)
{
	return calc_expo(num) == EXPO_MAX;
}

bool is_inf(struct my_double num)
{
	return is_special(num) && calc_frac(num) == 0uLL;
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
	return is_special(num) && calc_frac(num) > 0uLL;
}

struct my_double minus__my_double(struct my_double num)
{
	struct my_double res = num;
	set_sign_bit(&res, sign_bit(&num) ^ 1);
	return res;
}

struct my_double add__my_double(struct my_double a, struct my_double b)
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
		swap__my_double(&a, &b);
		swap__int(&E1, &E2);
	}
	struct my_int128 M1 = my_double_to_my_int128(a), M2 = my_double_to_my_int128(b);
	shift_to_pos_of_integer(&M1);
	shift_to_pos_of_integer(&M2);
	right_shift_k_bit(&M2, E1 - E2);
	int E = E1;
	struct my_int128 M;
	bool sign;
	if (sign_bit(&a) == sign_bit(&b))
	{
		sign = sign_bit(&a);
		M = add__my_int128(M1, M2);
	}
	else
	{
		if (cmp_abs__my_double(a, b) == SMALLER)
		{	
			swap__my_double(&a, &b);
			swap__my_int128(&M1, &M2);
		}
		if (cmp_abs__my_double(a, b) == EQUAL) sign = POS;
		else sign = sign_bit(&a);
		M = subtract__my_int128(M1, M2);
	}
	return adjust(sign, M, E);
}

struct my_double subtract__my_double(struct my_double a, struct my_double b)
{
	if (is_NaN(a)) return a;
	if (is_NaN(b)) return b;
	return add__my_double(a, minus__my_double(b));
}

struct my_double multiply__my_double(struct my_double a, struct my_double b)
{
	if (is_NaN(a)) return a;
	if (is_NaN(b)) return b;
	bool sign = sign_bit(&a) ^ sign_bit(&b);
	if (is_inf(a) && is_inf(b)) return gen_inf(sign);
	if (!is_inf(a) &&  is_inf(b)) swap__my_double(&a, &b);
	if ( is_inf(a) && !is_inf(b))
	{
		if (is_zero(b)) return gen_NaN(sign);
		return gen_inf(sign);
	}

	// now a, b are normalized or denormalized
	int E1 = calc_E(a), E2 = calc_E(b);
	int E = E1 + E2;
	struct my_int128 M1, M2, M;
	M1 = my_double_to_my_int128(a);
	M2 = my_double_to_my_int128(b);
	M = multiply__my_int128(M1, M2);
	return adjust(sign, M, E);
}

struct my_double divide__my_double(struct my_double a, struct my_double b)
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
	M1 = my_double_to_my_int128(a);
	M2 = my_double_to_my_int128(b);
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
	M = divide__my_int128(M1, M2);
	return adjust(sign, M, E);
}

struct my_double calculate__my_double(struct my_double a, char operator, struct my_double b)
{
	switch(operator)
	{
	case ADD:
		return add__my_double(a, b);
		break;
	case SUBTRACT:
		return subtract__my_double(a, b);
		break;
	case MULTIPLY:
		return multiply__my_double(a, b);
		break;
	case DIVIDE:
		return divide__my_double(a, b);
		break;
	default:
		printf("error: unknown operator\n");
		assert(FALSE);
		return gen_rand();
		break;
	}
}

int cmp__my_int128(struct my_int128 a, struct my_int128 b)
{
	for (int i = NUM_OF_BITS_IN_INT128 - 1; i >= 0; --i)
	{
		bool bit_a = kth_bit__my_int128(&a, i);
		bool bit_b = kth_bit__my_int128(&b, i);
		if (bit_a == 1 && bit_b == 0) return LARGER;
		if (bit_a == 0 && bit_b == 1) return SMALLER;
	}
	return EQUAL;
}

bool kth_bit__my_int128(struct my_int128* num, int k)
{
	if (k < NUM_OF_BITS_IN_LONG_LONG) return (bool) (((*num).se >> k) & 1);
	k -= NUM_OF_BITS_IN_LONG_LONG;
	return (bool) (((*num).fi >> k) & 1);
}

void set_kth_bit_long_long(unsigned long long* num, int k, bool bit)
{
	*num &= ULLONG_MAX ^ (1uLL << k);
	*num |= ((unsigned long long) (bit)) << k;
}

void set_kth_bit__my_int128(struct my_int128* num, int k, bool bit)
{
	if (k < NUM_OF_BITS_IN_LONG_LONG) return set_kth_bit_long_long(&((*num).se), k, bit);
	k -= NUM_OF_BITS_IN_LONG_LONG;
	set_kth_bit_long_long(&((*num).fi), k, bit);
}

void add_kth_bit__my_int128(struct my_int128* num, int k)
{
	for (; k < NUM_OF_BITS_IN_INT128 && kth_bit__my_int128(num, k) == 1; ++k)
	{
		set_kth_bit__my_int128(num, k, 0);
	}
	if (k < NUM_OF_BITS_IN_INT128) set_kth_bit__my_int128(num, k, 1);
}

void subtract_kth_bit__my_int128(struct my_int128* num, int k)
{
	for (; kth_bit__my_int128(num, k) == 0; ++k)
	{
		set_kth_bit__my_int128(num, k, 1);
	}
	set_kth_bit__my_int128(num, k, 0);
}

struct my_int128 gen_zero__my_int128()
{
	struct my_int128 res;
	res.fi = res.se = 0uLL;
	return res;
}

bool is_zero__my_int128(struct my_int128 num)
{
	return num.fi == 0uLL && num.se == 0uLL;
}

struct my_int128 add__my_int128(struct my_int128 a, struct my_int128 b)
{
	struct my_int128 res;
	res.fi = a.fi + b.fi;
	res.se = a.se + b.se;
	if (res.se < a.se) ++res.fi;
	return res;
}

struct my_int128 subtract__my_int128(struct my_int128 a, struct my_int128 b)
{
	struct my_int128 res;
	res.fi = a.fi - b.fi;
	res.se = a.se - b.se;
	if (res.se > a.se) --res.fi;
	return res;
}

struct my_int128 multiply__my_int128(struct my_int128 a, struct my_int128 b)
{
	struct my_int128 res;
	res = gen_zero__my_int128();
	for (int i = 0; i < NUM_OF_BITS_IN_LONG_LONG; ++i)
	{
		if (kth_bit__my_int128(&a, i) == 1)
		{
			for (int j = 0; j < NUM_OF_BITS_IN_LONG_LONG; ++j)
			{
				if (kth_bit__my_int128(&b, j) == 1)
				{
					add_kth_bit__my_int128(&res, i + j);
				}
			}
		}
	}
	return res;
}

struct my_int128 divide__my_int128(struct my_int128 a, struct my_int128 b)
{
	struct my_int128 res;
	res = gen_zero__my_int128();
	for (int i = POS_OF_INTEGER_IN_MY_INT128; i >= 0; --i, right_shift(&b))
	{
		if (cmp__my_int128(b, a) != LARGER)
		{
			set_kth_bit__my_int128(&res, i, 1);
			a = subtract__my_int128(a ,b);
		}
	}
	return res;
}

struct my_int128 my_double_to_my_int128(struct my_double num)
{
	struct my_int128 res;
	res.fi = 0uLL;
	res.se = calc_frac(num);
	if (is_normal(num)) res.se |= 1uLL << NUM_OF_BITS_OF_FRAC;
	return res;
}

unsigned long long my_int128_to_frac(struct my_int128 num)
{
	unsigned long long res = 0uLL;
	for (int i = POS_OF_INTEGER_IN_MY_INT128 - NUM_OF_BITS_OF_FRAC, k = 0; i < POS_OF_INTEGER_IN_MY_INT128; ++i, ++k)
	{
		bool bit = kth_bit__my_int128(&num, i);
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
		if (i - k >= 0) bit = kth_bit__my_int128(M, i - k);
		set_kth_bit__my_int128(M, i, bit);
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
		if (i + k < NUM_OF_BITS_IN_INT128) bit = kth_bit__my_int128(M, i + k);
		set_kth_bit__my_int128(M, i, bit);
	}
}

void shift_to_pos_of_integer(struct my_int128* M)
{
	left_shift_k_bit(M, POS_OF_INTEGER_IN_MY_INT128 - NUM_OF_BITS_OF_FRAC);
}

int calc_pos_of_first_bit(struct my_int128 num)
{
	for (int i = NUM_OF_BITS_IN_INT128 - 1; i >= 0; --i)
	{
		if (kth_bit__my_int128(&num, i) == 1)
		{
			return i;
		}
	}
	return NOT_FOUND;
}

bool rounding_type(struct my_int128 num)
{
	if (kth_bit__my_int128(&num, ROUND_BIT) == 0) return DOWN;
	for (int i = STICKY_BIT; i >= 0; --i)
	{
		if (kth_bit__my_int128(&num, i) == 1)
		{
			return UP;
		}
	}
	if (kth_bit__my_int128(&num, GUARD_BIT) == 1) return UP;
	return DOWN;
}

void rounding(struct my_int128* M, int* E)
{
	bool type = rounding_type(*M);
	if (type == UP)
	{
		add_kth_bit__my_int128(M, GUARD_BIT);
		if (kth_bit__my_int128(M, POS_OF_INTEGER_IN_MY_INT128 + 1) == 1)
		{
			right_shift(M);
			++(*E);
			if(*E > E_MAX)
			{
				(*M) = gen_zero__my_int128();
			}
		}
	}
}

struct my_double adjust(bool sign, struct my_int128 M, int E)
{
	int pos_of_first_bit = calc_pos_of_first_bit(M);
	if (pos_of_first_bit == NOT_FOUND) return gen_zero(sign);
	if (pos_of_first_bit > POS_OF_INTEGER_IN_MY_INT128)
	{
		int delta = pos_of_first_bit - POS_OF_INTEGER_IN_MY_INT128;
		right_shift_k_bit(&M, delta);
		E += delta;
	}
	else if (pos_of_first_bit < POS_OF_INTEGER_IN_MY_INT128)
	{
		int delta = POS_OF_INTEGER_IN_MY_INT128 - pos_of_first_bit;
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
	if (kth_bit__my_int128(&M, POS_OF_INTEGER_IN_MY_INT128) == 1) expo = E + BIAS;
	else expo = EXPO_MIN;
	unsigned long long frac = my_int128_to_frac(M);
	return gen_sign_expo_frac(sign, expo, frac);
}

// process a line of input
struct my_double evaluate(int buf[], int* buf_pos)
{
	static char str[LEN_OF_STR];
	int num_of_operator = 0, num_of_operand = 0;
	static int stk_operator[LEN_OF_STR];
	static struct my_double stk_operand[LEN_OF_STR];
	for (int type; (type = get_ope(buf, buf_pos, str)) != NEWLINE; )
	{
		switch(type)
		{
		case NUMBER:
			push_operand(stk_operand, &num_of_operand, string_to_my_double(str));
			break;
		case ADD:
		case SUBTRACT:
		case MULTIPLY:
		case DIVIDE:
		case LEFT_PARENTHESIS:
		case RIGHT_PARENTHESIS:
			push_operator(stk_operator, &num_of_operator, type, stk_operand, &num_of_operand);
			break;
		default:
			printf("error: unknown command %s\n", str);
			assert(FALSE);
			break;
		}
	}
	push_operator(stk_operator, &num_of_operator, TERMINATOR, stk_operand, &num_of_operand);
	return stk_operand[0];
}

int char_to_value(int c)
{
	return c - ZERO;
}

int value_to_char(int c)
{
	return ZERO + c;
}

int getch(int buf[], int* buf_pos)
{
	return (*buf_pos > 0) ? buf[--(*buf_pos)] : getchar();
}

void ungetch(int buf[], int* buf_pos, int c)
{
	buf[(*buf_pos)++] = c;
}

int get_ope(int buf[], int* buf_pos, char str[])
{
	int c;
	while ((str[0] = c = getch(buf, buf_pos)) == SPACE || c == TAB)
		;
	str[1] = NULL_CHARACTER;
	if (!isdigit(c) && c != DECIMAL_POINT && c != ADD && c != SUBTRACT)
		return c;
	if (c == ADD || c == SUBTRACT)
	{
		int nxt = getch(buf, buf_pos);
		if (nxt == SPACE || nxt == TAB) return c;
		ungetch(buf, buf_pos, nxt);
	}
	int i = 1;
	if (c == ADD || c == SUBTRACT) str[i++] = c = getch(buf, buf_pos);
	if (isdigit(c))
	{
		while (isdigit(str[i++] = c = getch(buf, buf_pos)))
			;
	}
	if (c == DECIMAL_POINT)
	{
		while (isdigit(str[i++] = c = getch(buf, buf_pos)))
			;
	}
	if (c == LOWER_E || c == UPPER_E) str[i++] = c = getch(buf, buf_pos);
	if (c == ADD || c == SUBTRACT) str[i++] = c = getch(buf, buf_pos);
	while (isdigit(str[i++] = c = getch(buf, buf_pos)))
		;
	str[i - 1] = NULL_CHARACTER;
	ungetch(buf, buf_pos, c);
	return NUMBER;
}

void push_operand(struct my_double stk_operand[], int* num_of_operand, struct my_double operand)
{
	stk_operand[(*num_of_operand)++] = operand;
}

void push_operator(int stk_operator[], int* num_of_operator, int operator, struct my_double stk_operand[], int* num_of_operand)
{
	switch(operator)
	{
	case LEFT_PARENTHESIS:
		stk_operator[(*num_of_operator)++] = operator;
		break;
	case RIGHT_PARENTHESIS:
		while (stk_operator[(*num_of_operator) - 1] != LEFT_PARENTHESIS)
		{
			calculate_top_stk(stk_operator, num_of_operator, stk_operand, num_of_operand);
		}
		--(*num_of_operator);
		break;
	default:
		while (*num_of_operator > 0 && stk_operator[(*num_of_operator) - 1] != LEFT_PARENTHESIS && priority(stk_operator[(*num_of_operator) - 1]) >= priority(operator))
		{
			calculate_top_stk(stk_operator, num_of_operator, stk_operand, num_of_operand);
		}
		stk_operator[(*num_of_operator)++] = operator;
		break;
	}
}

void calculate_top_stk(int stk_operator[], int* num_of_operator, struct my_double stk_operand[], int* num_of_operand)
{
	struct my_double tmp;
	tmp = calculate__my_double(stk_operand[(*num_of_operand - 2)], stk_operator[--(*num_of_operator)], stk_operand[(*num_of_operand - 1)]);
	(*num_of_operand) -= 2;
	push_operand(stk_operand, num_of_operand, tmp);
}

int priority(int operator)
{
	switch(operator)
	{
	case MULTIPLY:
	case DIVIDE:
		return 1;
		break;
	case ADD:
	case SUBTRACT:
		return 0;
		break;
	case TERMINATOR:
		return -1;
		break;
	default:
		printf("error: unknown operator %d\n", operator);
		assert(FALSE);
		break;
	}
}

bool has_integer_part__big_number(struct big_number* num)
{
	for (int i = POS_OF_INTEGER_IN_BIG_NUMBER; i >= 0; --i)
	{
		if ((*num).digits[i] > 0)
		{
			return TRUE;
		}
	}
	return FALSE;
}

void set_zero__big_number(struct big_number* num)
{
	for (int i = 0; i < NUM_OF_DIGITS_IN_BIG_NUMBER; ++i)
	{
		(*num).digits[i] = 0;
	}
}

void set_one__big_number(struct big_number* num)
{
	set_zero__big_number(num);
	set_kth_digit__big_number(num, POS_OF_INTEGER_IN_BIG_NUMBER, 1);
}

void left_shift_k_digits__big_number(struct big_number* num, int k)
{
	for (int i = 0; i < NUM_OF_DIGITS_IN_BIG_NUMBER; ++i)
	{
		int x = 0;
		if (i + k < NUM_OF_DIGITS_IN_BIG_NUMBER) x = (*num).digits[i + k];
		set_kth_digit__big_number(num, i, x);
	}
}

void right_shift_k_digits__big_number(struct big_number* num, int k)
{
	for (int i = NUM_OF_DIGITS_IN_BIG_NUMBER - 1; i >= 0; --i)
	{
		int x = 0;
		if (i - k >= 0) x = (*num).digits[i - k];
		set_kth_digit__big_number(num, i, x);
	}
}

void left_shift__big_number(struct big_number* num)
{
	left_shift_k_digits__big_number(num, 1);
}

void right_shift__big_number(struct big_number* num)
{
	right_shift_k_digits__big_number(num, 1);
}

void set_kth_digit__big_number(struct big_number* num, int k, int digit)
{
	(*num).digits[k] = digit;
}

void integer_part_divide_by_two__big_number(struct big_number* num)
{
	for (int i = 0; i <= POS_OF_INTEGER_IN_BIG_NUMBER; ++i)
	{
		int res = (*num).digits[i] / BASE_BINARY;
		if (((*num).digits[i] & 1) && i < POS_OF_INTEGER_IN_BIG_NUMBER)
		{
			(*num).digits[i + 1] += BASE_DECIMAL;
		}
		set_kth_digit__big_number(num, i, res);
	}
}

// return whether multiplication produces integer part or not
bool decimal_part_multiply_by_two__big_number(struct big_number* num)
{
	bool carry = FALSE;
	bool ret = FALSE;
	for (int i = NUM_OF_DIGITS_IN_BIG_NUMBER - 1; i > POS_OF_INTEGER_IN_BIG_NUMBER; --i)
	{
		int res = (*num).digits[i] * BASE_BINARY;
		if (carry == TRUE) ++res;
		if (res >= BASE_DECIMAL)
		{
			if (i == POS_OF_INTEGER_IN_BIG_NUMBER + 1)
			{
				ret = TRUE;
			}
			res -= BASE_DECIMAL;
			carry = TRUE;
		}
		else carry = FALSE;
		set_kth_digit__big_number(num, i, res);
	}
	return ret;
}

void multiply_by_two__big_number(struct big_number* num)
{
	bool carry = FALSE;
	for (int i = NUM_OF_DIGITS_IN_BIG_NUMBER - 1; i >= 0; --i)
	{
		int res = (*num).digits[i] * BASE_BINARY;
		if (carry == TRUE) ++res;
		if (res >= BASE_DECIMAL)
		{
			assert(i > 0);
			res -= BASE_DECIMAL;
			carry = TRUE;
		}
		else carry = FALSE;
		set_kth_digit__big_number(num, i, res);
	}
}

void divide_by_two__big_number(struct big_number* num)
{
	for (int i = 0; i < NUM_OF_DIGITS_IN_BIG_NUMBER; ++i)
	{
		int res = (*num).digits[i] / BASE_BINARY;
		if (((*num).digits[i] & 1) && i < NUM_OF_DIGITS_IN_BIG_NUMBER - 1)
		{
			(*num).digits[i + 1] += BASE_DECIMAL;
		}
		set_kth_digit__big_number(num, i, res);
	}
}

void add__big_number(struct big_number* a, struct big_number* b)
{
	for (int i = NUM_OF_DIGITS_IN_BIG_NUMBER - 1; i >= 0; --i)
	{
		int res = (*a).digits[i] + (*b).digits[i];
		if (res >= BASE_DECIMAL)
		{
			if (i > 0) ++((*a).digits[i - 1]);
			res -= BASE_DECIMAL;
		}
		set_kth_digit__big_number(a, i, res);
	}
}

int calc_pos_of_first_digit__big_number(struct big_number* num)
{
	for (int i = 0; i < NUM_OF_DIGITS_IN_BIG_NUMBER; ++i)
	{
		if ((*num).digits[i] > 0)
		{
			return i;
		}
	}
	return NOT_FOUND;
}

void init()
{
	srand(time(NULL));
	freopen(INPUT, READ_ONLY, stdin);
	freopen(OUTPUT, WRITE_ONLY, stdout);
}

void process()
{
	static int buf[BUF_SIZE];
	int buf_pos = 0, c;
	while (TRUE)
	{
		c = getch(buf, &buf_pos);
		if (c == EOF) break;
		ungetch(buf, &buf_pos, c);
		output__my_double(evaluate(buf, &buf_pos));
		putchar(NEWLINE);
	}
}

