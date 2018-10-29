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

#define NUM_OF_OPERATORS 4
#define ADD_ID 1
#define SUBTRACT_ID 2
#define MULTIPLY_ID 3
#define DIVIDE_ID 4

// bases
#define BASE_DECIMAL 10
#define BASE_BINARY  2

#define NUM_OF_DIGITS_OF_EXPO 3
#define PRECISION 30

// for input and output
#define READ_ONLY "r"
#define WRITE_ONLY "w"
#define OUTPUT "data.in"

#define NUM_OF_NUMBERS 10
#define NUM_OF_BRACKETS 5

#define CARRY_LIMIT 5

#define DOUBLE_FORMAT "%.50le"

struct my_double
{
	unsigned char bytes[NUM_OF_BYTES_IN_DOUBLE];
};

struct my_double gen_rand();
struct my_double gen_nonspecial_rand();

double my_double_to_double(struct my_double);

void swap__int(int*, int*);
void swap__my_double(struct my_double*, struct my_double*);

int index_kth_bit(int);
bool kth_bit(struct my_double*, int);

int calc_expo(struct my_double);

bool is_special(struct my_double);

void init();
void process();

void gen_expression();

int main()
{
	init();
	process();
}

struct my_double gen_rand()
{
	struct my_double res;
	for (int i = 0; i < NUM_OF_BYTES_IN_DOUBLE; ++i)
		res.bytes[i] = (unsigned char) (rand() % (UCHAR_MAX+1));
	return res;
}

struct my_double gen_nonspecial_rand()
{
	struct my_double res;
	do
	{
		res = gen_rand();
	}
	while (is_special(res));
	return res;
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

int calc_expo(struct my_double num)
{
	int res = 0;
	for (int i = NUM_OF_BITS_OF_FRAC, bit = 0; i < NUM_OF_BITS_IN_DOUBLE - 1; ++i, ++bit)
	{
		res |= ((unsigned char) (kth_bit(&num, i))) << bit;
	}
	return res;
}

bool is_special(struct my_double num)
{
	return calc_expo(num) == EXPO_MAX;
}

void init()
{
	srand(time(NULL));
	freopen(OUTPUT, WRITE_ONLY, stdout);
}

void process()
{
	gen_expression();
}

int rand_int(int L, int R)
{
	return rand() % (R - L + 1) + L;
}

void gen_expression()
{
	// generate numbers
	static struct my_double num[NUM_OF_NUMBERS];
	for (int i = 0; i < NUM_OF_NUMBERS; ++i)
	{
		num[i] = gen_nonspecial_rand();
	}
	
	// generate operators
	static int operator[NUM_OF_NUMBERS];
	for (int i = 0; i < NUM_OF_NUMBERS - 1; ++i)
	{
		int id = rand_int(1, NUM_OF_OPERATORS);
		switch(id)
		{
		case ADD_ID:
			operator[i] = ADD;
			break;
		case SUBTRACT_ID:
			operator[i] = SUBTRACT;
			break;
		case MULTIPLY_ID:
			operator[i] = MULTIPLY;
			break;
		case DIVIDE_ID:
			operator[i] = DIVIDE;
			break;
		dufault:
			assert(FALSE);
			break;
		}
	}
	
	// generate brackets
	static int num_of_left_brackets[NUM_OF_NUMBERS], num_of_right_brackets[NUM_OF_NUMBERS];
	memset(num_of_left_brackets , 0, NUM_OF_NUMBERS * sizeof(int));
	memset(num_of_right_brackets, 0, NUM_OF_NUMBERS * sizeof(int));
	for (int i = 0; i < NUM_OF_BRACKETS; ++i)
	{
		int L = rand_int(0, NUM_OF_NUMBERS - 1);
		int R = rand_int(0, NUM_OF_NUMBERS - 1);
		if (L > R) swap__int(&L, &R);
		++num_of_left_brackets[L];
		++num_of_right_brackets[R];
	}
	
	// output
	for (int i = 0; i < NUM_OF_NUMBERS; ++i)
	{
		for (int j = 0; j < num_of_left_brackets[i]; ++j)
			putchar(LEFT_PARENTHESIS);
		printf(DOUBLE_FORMAT, my_double_to_double(num[i]));
		for (int j = 0; j < num_of_right_brackets[i]; ++j)
			putchar(RIGHT_PARENTHESIS);
		if (i < NUM_OF_NUMBERS - 1)
		{
			putchar(SPACE);
			putchar(operator[i]);
			putchar(SPACE);
		}
		else putchar(NEWLINE);
	}
}

