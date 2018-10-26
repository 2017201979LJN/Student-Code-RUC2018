#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define bool char
#define NUM_OF_TEST 10
#define NUM_OF_TWO_POW 5000
#define NUM_OF_BYTES 8
#define NUM_OF_BIN 63
#define NUM_OF_FRAC 52
#define NUM_OF_EXP 11
#define NUM_OF_CHAR 10000
#define NUM_OF_BITS 64
#define BITS_IN_A_BYTE 8
#define BITS_OF_DOUBLE 700
#define HALF (BITS_OF_DOUBLE/2)

struct double_n
{
	unsigned char bytes[NUM_OF_BYTES];
};
//the sign of double stores at the left one bit in bytes[0]
//the exp of double stores at the right 7 bits in bytes[0] and the left 4 bits in bytes[1]
//the frac of double stores at the right 4 bits in bytes[1] and bytes[2 ~ 7]
long long bin[NUM_OF_BIN];
int bits_of_double_[BITS_OF_DOUBLE];
int bits_of_frac_of_c[NUM_OF_FRAC * 10];
int pow_of_2[NUM_OF_TWO_POW][BITS_OF_DOUBLE];
int bit[3][NUM_OF_FRAC * 10];
long long more,more1;
int bit_of_a[NUM_OF_BITS];
char str[NUM_OF_CHAR];
int Bias ;
void print_bits_of_double_n(struct double_n a)
{
	for (int i = 0; i < NUM_OF_BYTES; i++)
	{
		for (int j = BITS_IN_A_BYTE - 1; j >=0; j--)
		{
			printf("%lld", (a.bytes[i] & bin[j]) >> j);
		}
		printf(" ");
	}
	printf("\n");
}
int get_exp(struct double_n a)
{
	int exp = 0;
	exp = a.bytes[0] & (bin[BITS_IN_A_BYTE - 1] - 1);		//get the right 7 bits in bytes[0]
	exp <<= 4;
	exp |= ( a.bytes[1] & ( bin[BITS_IN_A_BYTE] - bin[4] ) ) >> 4;  //get the left 4 bits in bytes[1]
	return exp;
}
long long get_frac(struct double_n a)
{
	long long frac = a.bytes[1] & (bin[4] - 1);			//get the right 4 bits in bytes[1]
	for (int i = 2; i < NUM_OF_BYTES; i++)
	{
		frac <<= BITS_IN_A_BYTE;
		frac |= a.bytes[i];					//get bits in bytes[2 ~ 7]
	}
	return frac;
}
bool check_frac(struct double_n a)
{
	return get_frac(a) != 0LL;
}
int check_exp(struct double_n a)
{
	int exp_of_a = get_exp(a);
	if (exp_of_a == 0) return 0;
	if (exp_of_a == (bin[NUM_OF_EXP] - 1) ) return 2;
	return 1;
}
bool check_zero(struct double_n a)
{
	return (check_exp(a) == 0) && (check_frac(a) == 0);
}
bool check_inf(struct double_n a)
{
	return (check_exp(a) == 2) && (check_frac(a) == 0);
}
bool check_nan(struct double_n a)
{
	return (check_exp(a) == 2) && (check_frac(a) == 1);
}
bool check_denormalize(struct double_n a)
{
	return (check_exp(a) == 0) && (check_frac(a) != 0);
}
bool check_normalize(struct double_n a)
{
	return (check_exp(a) == 1) ;
}
int get_exponent(struct double_n a)
{
	return check_normalize(a) ? (get_exp(a) - Bias) : (1 - Bias);
}
int get_sign(struct double_n a)
{
	if(a.bytes[0] & bin[BITS_IN_A_BYTE - 1]) return -1;
	return 1;
}
struct double_n change_sign(struct double_n a,int sign_of_a)
{
	if (sign_of_a == 1) a.bytes[0] &= bin[BITS_IN_A_BYTE - 1] - 1;
	else a.bytes[0] |= bin[BITS_IN_A_BYTE - 1];	
	return a;
}
struct double_n change_exp(struct double_n a,int exp_of_a)
{
	struct double_n b = a;
	b.bytes[0] &= bin[BITS_IN_A_BYTE - 1] ;
	b.bytes[0] |= ( exp_of_a & ( bin[NUM_OF_EXP] - bin[4] ) ) >> 4;
	b.bytes[1] &= bin[4] - 1;
	b.bytes[1] |= (exp_of_a & (bin[4] - 1) ) << 4 ;
	return b;
}
struct double_n change_frac(struct double_n a,long long frac_of_a)
{
	struct double_n b = a;
	b.bytes[1] &= (bin[BITS_IN_A_BYTE] - bin[4]);
	b.bytes[1] |= ( frac_of_a & (bin[NUM_OF_FRAC] - bin[48]) ) >> 48 ;
	for (int i = 2; i < NUM_OF_BYTES - 1; i++)
	{
		b.bytes[i] = ( frac_of_a & ( bin[ (NUM_OF_BYTES - i) * BITS_IN_A_BYTE ] - bin[ (NUM_OF_BYTES - i - 1) * BITS_IN_A_BYTE ] ) ) >> ( (NUM_OF_BYTES - i - 1) * BITS_IN_A_BYTE);
	}
	b.bytes[NUM_OF_BYTES - 1] = frac_of_a & (bin[BITS_IN_A_BYTE] - 1) ;
	return b;
}
struct double_n right_shift(struct double_n a, int k)
{
	if (k == 0)
	{
		more1 = 0;
		more = -1;
		return a;
	}
	struct double_n b;
	int sign_of_b = get_sign(a);
	int exp_of_b = get_exp(a) + k;
	long long frac_of_a = get_frac(a) , frac_of_b;
	if (check_normalize(a) )
	frac_of_a += bin[NUM_OF_FRAC];
	frac_of_b = ( frac_of_a & (bin[NUM_OF_BIN - 1] - bin[k]) ) >> k;
	more = frac_of_a & (bin[k] - 1);
	more1 = more;
	if ( more == 0LL ) more = -1;
	else if ( more > bin[k - 1]) more = 2;
	else if ( more == bin[k - 1]) more = 1;
	else more = 0;
	b = change_sign(b, sign_of_b);
	b = change_exp(b, exp_of_b);
	b = change_frac(b, frac_of_b);
	return b;
}
bool compare(struct double_n a,struct double_n b)
{
	int exp_of_a = get_exp(a), exp_of_b = get_exp(b);
	if( exp_of_a != exp_of_b ) return exp_of_a < exp_of_b ;
	long long frac_of_a = get_frac(a), frac_of_b = get_frac(b);
	return frac_of_a < frac_of_b ;
}
struct double_n same_sign(struct double_n a,struct double_n b)
{
	if ( compare(a, b) )
	{
		struct double_n t = a;
		a = b;
		b = t;
	}
	struct double_n c;
	c = change_sign(c, get_sign(a) );
	int exp_of_a = get_exp(a), exp_of_b = get_exp(b);
	if (check_normalize(a) && check_normalize(b) )
	{
		if (exp_of_a == exp_of_b)
		{
			long long frac_of_a = get_frac(a), frac_of_b = get_frac(b);
			int exp_of_c = exp_of_a + 1;
		       	long long frac_of_c = (frac_of_a + frac_of_b) / 2;
			if ( (frac_of_a + frac_of_b) & 1)
			{
				if (frac_of_c & 1) frac_of_c ++;
			}
			if (exp_of_c >= bin[NUM_OF_EXP])
			{
				exp_of_c = bin[NUM_OF_EXP] - 1;
				frac_of_c = 0;
			}
			c = change_exp(c, exp_of_c);
			c = change_frac(c, frac_of_c);
			return c;
		}
		else
		{
			if (exp_of_b + NUM_OF_FRAC + 3 >= exp_of_a)
			{
				b = right_shift(b, exp_of_a - exp_of_b);
				long long frac_of_a = get_frac(a), frac_of_b = get_frac(b);
				if(frac_of_a + frac_of_b >= bin[NUM_OF_FRAC] )
				{
					int exp_of_c = exp_of_a + 1;
					long long frac_of_c = (frac_of_a + frac_of_b - bin[NUM_OF_FRAC] ) / 2 ;
					if ( (frac_of_a + frac_of_b - bin[NUM_OF_FRAC]) % 2 == 1)
					{
						if (more >= 0) frac_of_c ++;
						else
						{
							if (frac_of_c & 1) frac_of_c ++;
						}
					}
					c = change_exp(c , exp_of_c);
					c = change_frac(c , frac_of_c);
					return c;
				}
				else
				{
					int exp_of_c = exp_of_a ;
					long long frac_of_c = frac_of_a + frac_of_b ;
					if (more == 1)
					{
						if (frac_of_c & 1) frac_of_c ++;
					}
					if (more > 1)
					{
						frac_of_c ++;
					}
					c = change_exp(c, exp_of_c);
					c = change_frac(c, frac_of_c);
					return c;
				}
			}
			else
			{
			        return a;
			}
		}
	}
	else if (check_denormalize(a) && check_denormalize(b) )
	{
		long long frac_of_a = get_frac(a), frac_of_b = get_frac(b);
		if (frac_of_a + frac_of_b >= bin[NUM_OF_FRAC])
		{
			int exp_of_c = 1;
			long long frac_of_c = frac_of_a + frac_of_b - bin[NUM_OF_FRAC];
			c = change_exp(c, exp_of_c);
			c = change_frac(c, frac_of_c);
			return c;
		}
		else
		{
			int exp_of_c = 0;
			long long frac_of_c = frac_of_a + frac_of_b;
			c = change_exp(c, exp_of_c);
			c = change_frac(c, frac_of_c);
			return c;
		}
	}
	else if ( check_denormalize(b) )
	{
		int exp_of_a = get_exp(a), exp_of_b = get_exp(b);
		if ( exp_of_b + NUM_OF_FRAC + 3 >= exp_of_a)
		{
			b = right_shift(b, exp_of_a - exp_of_b - 1);
			long long frac_of_a = get_frac(a), frac_of_b = get_frac(b);
			if(frac_of_a + frac_of_b >= bin[NUM_OF_FRAC] )
			{
				int exp_of_c = exp_of_a + 1;
				long long frac_of_c = (frac_of_a + frac_of_b - bin[NUM_OF_FRAC] ) / 2 ;
				if ( (frac_of_a + frac_of_b - bin[NUM_OF_FRAC] ) % 2 == 1)
				{
					if (more >= 0) frac_of_c ++;
					if (more == -1)
					{
						if (frac_of_c % 2 == 1) frac_of_c ++;
					}
				}
				c = change_exp(c , exp_of_c);
				c = change_frac(c , frac_of_c);
				return c;
			}
			else
			{
				int exp_of_c = exp_of_a ;
				long long frac_of_c = frac_of_a + frac_of_b ;
				if (more > 1) frac_of_c ++;
				if (more == 1)
				{
					if (frac_of_c % 2 == 1) frac_of_c ++;
				}
				c = change_exp(c, exp_of_c);
				c = change_frac(c, frac_of_c);
				return c;
			}
		}
		else
		{
			return a;
		}
	}
}
struct double_n different_sign(struct double_n a, struct double_n b)
{
	if ( compare(a,b) ) 
	{
		struct double_n t = a;
		a = b;
		b = t;
	}
	int sign_of_c = get_sign(a);
	struct double_n c;
	c = change_sign(c, sign_of_c);
	if (check_normalize(a) && check_normalize(b) )
	{
		int exp_of_a = get_exp(a), exp_of_b = get_exp(b);
		if (exp_of_a == exp_of_b)
		{
			long long frac_of_c = get_frac(a) - get_frac(b);
			int exp_of_c = exp_of_a;
			while (frac_of_c < bin[NUM_OF_FRAC] && exp_of_c != 0)
			{
				frac_of_c *= 2;
				exp_of_c --;
			}
			if (exp_of_c == 0)
			{
				if (frac_of_c < bin[NUM_OF_FRAC])
				{
					c = change_exp(c, 0);
					c = change_frac(c, frac_of_c);
					return c;
				}
				else
				{
					c = change_exp(c, 1);
					c = change_frac(c, frac_of_c - bin[NUM_OF_FRAC]);
					return c;
				}
			}
			frac_of_c -= bin[NUM_OF_FRAC];
			c = change_exp(c, exp_of_c);
			c = change_frac(c, frac_of_c);
			return c;
		}
		if (exp_of_b + NUM_OF_FRAC + 3 < exp_of_a) return a;
		b = right_shift(b, exp_of_a - exp_of_b);
		long long frac_of_a = get_frac(a), frac_of_b = get_frac(b);
		int id = exp_of_a - exp_of_b - 1;
		if (frac_of_a < frac_of_b)
		{
			long long frac_of_c = frac_of_a - frac_of_b + bin[NUM_OF_FRAC];
			int exp_of_c = exp_of_a;
			while (frac_of_c < bin[NUM_OF_FRAC] && exp_of_c > 1)
			{
				frac_of_c *= 2;
				exp_of_c --;
				if (id >= 0 && more1 >= bin[id])
				{
					frac_of_c --;
					more1 -= bin[id];
				}
				id --;
			}
			if (exp_of_c == 1)
			{
				if ( frac_of_c < bin[NUM_OF_FRAC])
				{
					exp_of_c = 0;
					c = change_exp(c, exp_of_c);
					c = change_frac(c, frac_of_c);
				}
				else
				{
					c = change_exp(c, 1);
					c = change_frac(c, frac_of_c - bin[NUM_OF_FRAC]);
				}
				return c;
			}
			frac_of_c -= bin[NUM_OF_FRAC];
			if (more1 > bin[id] && id >= 0) frac_of_c --;
			if (more1 == bin[id] && frac_of_c % 2 == 1 && id >= 0) frac_of_c --;
			c = change_exp(c, exp_of_c);
			c = change_frac(c, frac_of_c);
			return c;
		}
		else
		{
			long long frac_of_c = frac_of_a - frac_of_b;
			int exp_of_c = exp_of_a;
			if (more1 > bin[id] && id >= 0) frac_of_c --;
			if (more1 == bin[id] && frac_of_c % 2 == 1 && id >= 0) frac_of_c --;
			c = change_exp(c, exp_of_c);
			c = change_frac(c, frac_of_c);
			return c;
		}
	}
	else if (check_denormalize(a) && check_denormalize(b) )
	{
		int exp_of_c = get_exp(a);
		long long frac_of_c = get_frac(a) - get_frac(b);
		c = change_exp(c, exp_of_c);
		c = change_frac(c, frac_of_c);
		return c;
	}
	else if (check_denormalize(b) )
	{
		int exp_of_a = get_exp(a), exp_of_b = get_exp(b);
		if (exp_of_b + NUM_OF_FRAC + 3 < exp_of_a) return a;
		b = right_shift(b, exp_of_a - exp_of_b - 1);
		long long frac_of_a = get_frac(a), frac_of_b = get_frac(b);
		int id = exp_of_a - exp_of_b - 2;
		if (frac_of_a < frac_of_b)
		{
			long long frac_of_c = frac_of_a - frac_of_b + bin[NUM_OF_FRAC];
			int exp_of_c = exp_of_a;
			if (exp_of_c == 1) exp_of_c --;
			while (frac_of_c < bin[NUM_OF_FRAC] && exp_of_c >= 1)
			{
				frac_of_c *= 2;
				exp_of_c --;
				if (id >= 0 && more1 >= bin[id])
				{
					more1 -= bin[id];
					frac_of_c --;
				}
				id --;
			}
			if (exp_of_c == 0)
			{
				if (frac_of_c < bin[NUM_OF_FRAC])
				{
					c = change_exp(c, exp_of_c);
					c = change_frac(c, frac_of_c);
				}
				else
				{
					c = change_exp(c, 1);
					c = change_frac(c, frac_of_c - bin[NUM_OF_FRAC]);
				}
				return c;
			}
			frac_of_c -= bin[NUM_OF_FRAC];
			if (more1 > bin[id]) frac_of_c --;
			if (more1 == bin[id] && id >= 0 && frac_of_c % 2 == 1) frac_of_c --;
			c = change_exp(c, exp_of_c);
			c = change_frac(c, frac_of_c);
			return c;
		}
		else
		{
			long long frac_of_c = frac_of_a - frac_of_b;
			int exp_of_c = exp_of_a;
			if (more1 > bin[id]) frac_of_c --;
			if (more1 == bin[id] && id >= 0 && frac_of_c % 2 == 1) frac_of_c --;
			c = change_exp(c, exp_of_c);
			c = change_frac(c, frac_of_c);
			return c;
		}
	}
}
struct double_n plus(struct double_n a, struct double_n b)
{
	if ( get_sign(a) == get_sign(b) )
	{
		return same_sign(a,b);
	}
	else
	{
		return different_sign(a,b);
	}
}
struct double_n minus(struct double_n a, struct double_n b)
{
	b = change_sign(b, get_sign(b) * (-1) );
	return plus(a, b);
}
// bit[0] shows bits of frac_of_a, bit[1] shows bits of frac_of_b, and bit[2] shows bits of frac_of_c
long long get_frac_of_c(long long frac_of_a , long long frac_of_b )
{
	for (int i = 1; i <= NUM_OF_FRAC; i++)
	bit[0][NUM_OF_FRAC + 1 - i] = (bin[i - 1] & frac_of_a) ? 1 : 0;
	for (int i = 1; i <= NUM_OF_FRAC; i++)
	bit[1][NUM_OF_FRAC + 1 - i] = (bin[i - 1] & frac_of_b) ? 1 : 0;
	bit[0][0] = (frac_of_a >= bin[NUM_OF_FRAC]);
	bit[1][0] = (frac_of_b >= bin[NUM_OF_FRAC]);
	for (int i = 0; i <= NUM_OF_FRAC * 2; i++)
	bit[2][i] = 0;
	for (int i = 0; i <= NUM_OF_FRAC; i++)
	{
		for (int j = 0; j <= NUM_OF_FRAC; j++)
		{
			bit[2][i + j] += bit[0][i] * bit[1][j];
		}
	}
	for (int i = NUM_OF_FRAC * 2; i > 0; i--)
	{
		bit[2][i - 1] += bit[2][i] / 2;
		bit[2][i] %= 2;
	}
	long long frac_of_c = 0LL;
	for (int i = NUM_OF_FRAC; i >= 0; i--)
	{
		frac_of_c += bin[NUM_OF_FRAC - i] * bit[2][i] ;
	}
	more = 0LL;
	for (int i = NUM_OF_FRAC * 2; i > NUM_OF_FRAC; i--)
	{
		more += bin[NUM_OF_FRAC * 2 - i] * bit[2][i];
	}
	return frac_of_c;
}
struct double_n multiply(struct double_n a, struct double_n b)
{
	struct double_n c;
	if (check_normalize(a) && check_normalize(b) )
	{
		int exp_of_a = get_exp(a), exp_of_b = get_exp(b) ;
		long long frac_of_a = get_frac(a), frac_of_b = get_frac(b);
		int exp_of_c = exp_of_a + exp_of_b - Bias;
		int sign_of_c = get_sign(a) * get_sign(b);
		long long frac_of_c = get_frac_of_c(frac_of_a + bin[NUM_OF_FRAC], frac_of_b + bin[NUM_OF_FRAC]);
		if (exp_of_c >= bin[NUM_OF_EXP] - 1) 
		{
			exp_of_c = bin[NUM_OF_EXP] - 1, frac_of_c = 0;
			c = change_sign(c, sign_of_c);
			c = change_exp(c, exp_of_c);
			c = change_frac(c, frac_of_c);
			return c;
		}
		if (exp_of_c < 0)
		{
			if (exp_of_c < -2 * NUM_OF_FRAC)
			{
				c = change_sign(c, sign_of_c);
				c = change_exp(c, 0);
				c = change_frac(c, 0);
			}
			else
			{
				int fl = 0;
				if (more > 0) fl = 1;
				while ( exp_of_c <= -1 )
				{
					exp_of_c ++;
					if (frac_of_c % 2 == 1)
					{
						fl = 1;
					}
					frac_of_c /= 2;
				}
				if (frac_of_c % 2 == 1)
				{
					frac_of_c /= 2;
					if (fl)
					frac_of_c ++;
					else
					{
						if (frac_of_c % 2 == 1) frac_of_c ++;
					}
				}
				else
				{
					frac_of_c /= 2;
				}
				c = change_sign(c, sign_of_c);
				c = change_exp(c, exp_of_c);
				c = change_frac(c, frac_of_c);
			}
			return c;
		}
		if (frac_of_c >= bin[NUM_OF_FRAC + 1]) 
		{
			more += bin[NUM_OF_FRAC] * (frac_of_c % 2 == 1);
			frac_of_c /= 2, exp_of_c ++ ;
			if (more > bin[NUM_OF_FRAC])
			{
				frac_of_c ++;
			}
			if (more == bin[NUM_OF_FRAC] && frac_of_c % 2 == 1)
			{
				frac_of_c ++;
			}
			if (exp_of_c >= bin[NUM_OF_EXP] - 1)
			{
				exp_of_c = bin[NUM_OF_EXP] - 1;
				frac_of_c = 0;
			}
			c = change_sign(c, sign_of_c);
			c = change_exp(c, exp_of_c);
			c = change_frac(c, frac_of_c);
			return c;
		}
		if (exp_of_c == 0)
		{
			int f = 0;
			if (frac_of_c % 2 == 1)
			{
				f = 1;
			}
			frac_of_c /= 2;
			if (f == 1)
			{
				if (more > 0)
				frac_of_c ++;
				else if(frac_of_c % 2 == 1)
				frac_of_c ++;
			}
			if (frac_of_c >= bin[NUM_OF_FRAC])
			{
				exp_of_c = 1;
				frac_of_c -= bin[NUM_OF_FRAC];
			}
			c = change_sign(c , sign_of_c);
			c = change_exp(c , exp_of_c);
			c = change_frac(c , frac_of_c);
			return c;
		}
		if (more > bin[NUM_OF_FRAC - 1]) frac_of_c ++;
		if (more == bin[NUM_OF_FRAC - 1] && frac_of_c % 2 == 1) frac_of_c ++;
		if (exp_of_c >= bin[NUM_OF_EXP] - 1) exp_of_c = bin[NUM_OF_EXP] - 1, frac_of_c = 0;
		else if(exp_of_c != 0) frac_of_c -= bin[NUM_OF_FRAC];
		c = change_sign(c , sign_of_c);
		c = change_exp(c , exp_of_c);
		c = change_frac(c , frac_of_c);
		return c;
	}
	if (check_denormalize(a) && check_denormalize(b) )
	{
		int sign_of_a = get_sign(a), sign_of_b = get_sign(b);
		int sign_of_c = sign_of_a * sign_of_b;
		c = change_sign(c, sign_of_c);
		c = change_exp(c, 0);
		c = change_frac(c, 0);
		return c;
	}
	if (check_denormalize(a) || check_denormalize(b) )
	{
		if (check_denormalize(a) )
		{
			struct double_n t = a;
			a = b;
			b = t;
		}
		int exp_of_a = get_exp(a);
		int exp_of_c = exp_of_a + 1 - Bias;
		int sign_of_c = get_sign(a) * get_sign(b);
		c = change_sign(c, sign_of_c);
		if (exp_of_a < 2 * Bias - 1)
		{
			c = change_exp(c, 0);
			c = change_frac(c, 0);
			return c;
		}
		long long frac_of_a = get_frac(a), frac_of_b = get_frac(b);
		long long frac_of_c = get_frac_of_c(frac_of_a + bin[NUM_OF_FRAC], frac_of_b);
		if (frac_of_c >= bin[NUM_OF_FRAC + 1])
		{
			if (frac_of_c % 2 == 1)
			{
				if (more >= 0)
				more = 2;
				else
				more = 1;
			}
			frac_of_c /= 2, exp_of_c ++ ;
			if (more == 2) frac_of_c ++;
			if (more == 1 && frac_of_c % 2 == 1) frac_of_c ++;
		}
		while (frac_of_c < bin[NUM_OF_FRAC])
		{
			exp_of_c --, frac_of_c *= 2;
			if (frac_of_c >= bin[NUM_OF_FRAC])
			{
				break;
			}
			if (exp_of_c < 0)
			{
				exp_of_c = 0;
				frac_of_c = 0;
				break;
			}
		}
		if (exp_of_c >= bin[NUM_OF_EXP] - 1) exp_of_c = bin[NUM_OF_EXP] - 1, frac_of_c = 1;
		if (exp_of_c > 0) frac_of_c -= bin[NUM_OF_FRAC];
		c = change_exp(c, exp_of_c);
		c = change_frac(c, frac_of_c);
		return c;
	}
}
long long get_frac_of_double_n_c(long long feng_zi, long long feng_mu)
{
	long long frac_of_c = 0LL;
	for (int i = 1; i <= NUM_OF_FRAC; i++)
	{
		feng_zi *= 2;
		if (feng_zi >= feng_mu)
		{
			frac_of_c += bin[NUM_OF_FRAC - i];
			feng_zi -= feng_mu;
		}
		else
		{
			bits_of_frac_of_c[i] = 0;
		}
	}
	if (feng_zi * 2 > feng_mu) frac_of_c ++;
	if (feng_zi * 2 == feng_mu && frac_of_c % 2 == 1) frac_of_c ++;
	return frac_of_c;
}
struct double_n divide_by(struct double_n a, struct double_n b)
{
	struct double_n c;
	int sign_of_c = get_sign(a) * get_sign(b);
	c = change_sign(c, sign_of_c);
	long long feng_zi,feng_mu;
	int exp_of_c;
	if (check_normalize(a) && check_normalize(b) )
	{
		int exp_of_a = get_exp(a), exp_of_b = get_exp(b);
		long long frac_of_a = get_frac(a), frac_of_b = get_frac(b);
		exp_of_c = exp_of_a - exp_of_b + Bias;
		feng_zi = frac_of_a + bin[NUM_OF_FRAC], feng_mu = frac_of_b + bin[NUM_OF_FRAC];
		while (feng_zi < feng_mu)
		{
			feng_zi *= 2;
			exp_of_c --;
		}
	}
	if (check_normalize(a) && check_denormalize(b) )
	{
		int exp_of_a = get_exp(a);
		exp_of_c = exp_of_a - 1 + Bias;
		long long frac_of_a = get_frac(a), frac_of_b = get_frac(b);
		feng_zi = frac_of_a + bin[NUM_OF_FRAC], feng_mu = frac_of_b;
		while (feng_zi >= feng_mu * 2)
		{
			feng_mu *= 2;
			exp_of_c ++;
		}
	}
	if (check_denormalize(a) && check_normalize(b) )
	{
		int exp_of_b = get_exp(b);
		exp_of_c = 1 - exp_of_b + Bias;
		long long frac_of_a = get_frac(a), frac_of_b = get_frac(b);
		feng_zi = frac_of_a, feng_mu = frac_of_b + bin[NUM_OF_FRAC];
		while (feng_zi < feng_mu)
		{
			feng_zi *= 2;
			exp_of_c --;
		}
	}
	if (check_denormalize(a) && check_denormalize(b) )
	{
		exp_of_c = Bias;
		long long frac_of_a = get_frac(a), frac_of_b = get_frac(b);
		while (frac_of_a < frac_of_b)
		{
			frac_of_a *= 2;
			exp_of_c --;
		}
		while (frac_of_a >= frac_of_b * 2)
		{
			frac_of_b *= 2;
			exp_of_c ++;
		}
		feng_zi = frac_of_a;
		feng_mu = frac_of_b;
	}
	long long frac_of_c = get_frac_of_double_n_c(feng_zi - feng_mu, feng_mu);
	c = change_exp(c, exp_of_c);
	c = change_frac(c, frac_of_c);
	return c;
}
struct double_n special_calculate(struct double_n a,char op,struct double_n b)
{
	if (check_normalize(a) || check_denormalize(a) )
	{
		struct double_n t = a;
		a = b;
		b = t;
	}
	int sign_of_a = get_sign(a), sign_of_b = get_sign(b);
	int new_sign = sign_of_a * sign_of_b;
	struct double_n c,_nan,nan;
	nan = change_sign(nan, 1);
	nan = change_exp(nan, bin[NUM_OF_EXP] - 1);
	nan = change_frac(nan, 1);
	_nan = change_sign(nan, -1);
	if (check_inf(a))
	{
		c = a;
		c = change_sign(c, new_sign);
		if (check_inf(b))
		{
			if (op == '-' && sign_of_a == sign_of_b) return _nan; 
			if (op == '+' && sign_of_a != sign_of_b) return _nan;
			if (op == '/') return _nan;
			return c;
		}
		if (check_nan(b))
		{
			return b;
		}
		if (check_zero(b))
		{
			if (op == '*') return _nan;
			return c;
		}
		return c;
	}
	if (check_nan(a) )
	{
		return a;
	}
	if (check_zero(a) )
	{
		c = change_sign(c, new_sign);
		if (check_nan(b) ) return b;
		if (check_inf(b) )
		{
			if (op == '*') return _nan;
			if (op == '-') return b = change_sign(b, get_sign(b) * (-1) );
			if (op == '+') return b;
			if (op == '/') return a = change_sign(a, new_sign);
		}
		if (check_zero(b) )
		{
			if (op == '/') return _nan;
			if (op == '+' && get_sign(a) != get_sign(b) ) return a = change_sign(a, 1);
			if (op == '-' && get_sign(a) == get_sign(b) ) return a = change_sign(a, 1);
			if (op == '*') return a = change_sign(a, new_sign);
			return a;
		}
		if (op == '+') return b;
		if (op == '-') return b = change_sign(b, get_sign(b) * (-1) );
		if (op == '*') return a = change_sign(a, new_sign);
		if (op == '/') return change_sign(a, new_sign);
	}
}
struct double_n calculate(struct double_n a, char op, struct double_n b)
{
	if (check_inf(a) || check_nan(a) || check_zero(a) || check_inf(b) || check_nan(b) || check_zero(b) )
	{
		return special_calculate(a,op,b);
	}
	if (op == '+') return plus(a, b);
	if (op == '-') return minus(a, b);
	if (op == '*') return multiply(a, b);
	if (op == '/') return divide_by(a, b);
}
bool check_op(char a)
{
	return a == '+' || a == '-' || a == '*' || a == '/' || a == '(' || a == ')';
}
int check_front()
{
	bool flag = 1;
	for (int i = HALF - 1; i >= 0; i--)
	{
		if (bits_of_double_[i] != 0)
		{
			flag = 0;
			break;
		}
	}
	if (flag == 0 || bits_of_double_[HALF] > 1) return 2;
	if (flag == 1 && bits_of_double_[HALF] == 1) return 1;
	return 0;
}
void double_mul_2()
{
	bool flag = 0;
	for (int i = BITS_OF_DOUBLE - 1; i >= 0; i--)
	{
		bits_of_double_[i] *= 2;
		if (flag)
		{
			bits_of_double_[i] ++;
			flag = 0;
		}
		if (bits_of_double_[i] >= 10)
		{
			bits_of_double_[i] -= 10;
			flag = 1;
		}
	}
}
void double_div_2()
{
	for (int i = 0; i < BITS_OF_DOUBLE; i++)
	{
		if (bits_of_double_[i] % 2 == 1)
		{
			bits_of_double_[i + 1] += 10;
		}
		bits_of_double_[i] /= 2;
	}
}
void get_pow_of_2()
{
	for (int i = 0; i < BITS_OF_DOUBLE; i++)
	{
		pow_of_2[Bias][i] = 0;
		bits_of_double_[i] = 0;
	}
	pow_of_2[Bias][HALF] = 1;
	bits_of_double_[HALF] = 1;
	for (int i = Bias + 1; i < NUM_OF_TWO_POW; i++)
	{
		double_mul_2();
		for (int j = 0; j < BITS_OF_DOUBLE; j++)
		{
			pow_of_2[i][j] = bits_of_double_[j];
		}
	}
	memset(bits_of_double_,0,sizeof(bits_of_double_) );
	bits_of_double_[HALF] = 1;
	for (int i = Bias - 1; i >= 1; i--)
	{
		double_div_2();
		for (int j = 0; j < BITS_OF_DOUBLE; j++)
		{
			pow_of_2[i][j] = bits_of_double_[j];
		}
	}
}
bool bigger_than(int a)
{
	for (int i = 0; i < BITS_OF_DOUBLE; i++)
	{
		if (bits_of_double_[i] != pow_of_2[a][i]) return bits_of_double_[i] > pow_of_2[a][i];
	}
	return 1;
}
void minus_pow_of_two(int a)
{
	for (int i = BITS_OF_DOUBLE - 1; i >= 0; i--)
	{
		if (bits_of_double_[i] < pow_of_2[a][i])
		{
			bits_of_double_[i] += 10 - pow_of_2[a][i];
			bits_of_double_[i - 1] --;
		}
		else
		{
			bits_of_double_[i] -= pow_of_2[a][i];
		}
	}
}
void add_pow_of_two(int a)
{
	for (int i = BITS_OF_DOUBLE - 1; i >= 0; i--)
	{
		bits_of_double_[i] += pow_of_2[a][i];
		if (bits_of_double_[i] >= 10)
		{
			bits_of_double_[i] -= 10;
			bits_of_double_[i - 1] ++;
		}
	}
}
struct double_n get_exp_and_frac_of_double_n(struct double_n a)
{
	int exp_of_a = Bias;
	while (check_front() == 2)
	{
		double_div_2();
		exp_of_a ++;
	}
	while (check_front() == 0)
	{
		double_mul_2();
		exp_of_a --;
		if (exp_of_a == 1)
		{
			if(check_front() == 0)
			exp_of_a = 0;
			break;
		}
	}
	long long frac_of_a = 0LL;
	bits_of_double_[HALF] = 0;
	for (int i = 1; i <= NUM_OF_FRAC; i++)
	{
		if (bigger_than(Bias - i) )
		{
			frac_of_a += bin[NUM_OF_FRAC - i];
			minus_pow_of_two(Bias - i);
		}
	}
	if (bigger_than(Bias - NUM_OF_FRAC - 1)) frac_of_a ++;
	a = change_exp(a, exp_of_a);
	a = change_frac(a, frac_of_a);
	return a;
}
struct double_n get_type_1(int l,int r)
{
	struct double_n a;
	if (str[l] == '-')
	{
		a = change_sign(a, -1);
		l++;
	}	
	else
	{
		a = change_sign(a, 1);
		if (str[l] == '+') l++;
	}
	int pl = -1;
	for (int i = l; i <= r; i++)
	{
		if (str[i] == '.') 
		{
			pl = i;
			break;
		}
	}
	if (pl == -1) pl = r + 1;
	memset(bits_of_double_, 0, sizeof(bits_of_double_) );
	for (int i = pl - 1; i >= l; i--)
	{
		bits_of_double_[HALF - (pl - 1 - i)] = str[i] - '0'; 
	}
	for (int i = pl + 1; i <= r; i++)
	{
		bits_of_double_[HALF + i - pl] = str[i] - '0';
	}
	a = get_exp_and_frac_of_double_n(a);
	return a;
}
struct double_n get_type_2(int l,int r)
{
	int sign_of_a = 1;
	struct double_n a;
	if (str[l] == '-') sign_of_a = -1;
	int st = -1;
	for (int i = r; i >= l; i--)
	{
		if (str[i] == 'E' || str[i] == 'e')
		{
			st = i;
			break;
		}
	}
	int e = 0, sign = 1, ed = st - 1;
	if (str[st + 1] == '-')
	{
		sign = -1;
		st ++;
	}
	st ++;
	for (int i = st; i <= r; i++)
	e = e * 10 + str[i] - '0';
	e = e * sign;
	memset(bits_of_double_, 0, sizeof(bits_of_double_) );
	bits_of_double_[HALF - e] = str[l] - '0';
	for (int i = l + 2; i <= ed; i++)
	bits_of_double_[HALF - e + i - l - 1] = str[i] - '0';
	a = change_sign(a, sign_of_a);
	a = get_exp_and_frac_of_double_n(a);
	return a;
}
struct double_n get_double_n(int l,int r)
{
	for (int i = l; i <= r; i++)
	{
		if (str[i] == 'e' || str[i] == 'E') return get_type_2(l,r);
	}
	return get_type_1(l,r);
}
void print_out_double_n(struct double_n a, int k)
{
	int exp_of_a = get_exp(a);
	long long frac_of_a = get_frac(a);
	memset(bits_of_double_, 0, sizeof(bits_of_double_));
	if (exp_of_a == 0)
	{
		for (int i = 1; i <= NUM_OF_FRAC; i++)
		{
			if (frac_of_a >= bin[NUM_OF_FRAC - i])
			{
				frac_of_a -= bin[NUM_OF_FRAC - i];
				add_pow_of_two(Bias - i);
			}
		}
		exp_of_a = 1;
	}
	else
	{
		add_pow_of_two(Bias);
		for (int i = 1; i <= NUM_OF_FRAC; i++)
		{
			if (frac_of_a >= bin[NUM_OF_FRAC - i])
			{
				frac_of_a -= bin[NUM_OF_FRAC - i];
				add_pow_of_two(Bias - i);
			}
		}
	}
	while (exp_of_a < Bias)
	{
		double_div_2();
		exp_of_a ++;
	}
	while (exp_of_a > Bias)
	{
		double_mul_2();
		exp_of_a --;
	}
	int st = -1, ed;
	for (int i = 0; i < BITS_OF_DOUBLE; i++)
	{
		if (bits_of_double_[i])
		{
			st = i;
			break;
		}
	}
	for (int i = BITS_OF_DOUBLE - 1; i >= 0; i--)
	{
		if (bits_of_double_[i])
		{
			ed = i;
			break;
		}
	}
	if (st == -1)
	{
		printf("0\n");
		return ;
	}
	int pl = st + k + 1;
	if (bits_of_double_[pl] > 5)
	bits_of_double_[--pl] ++;
	else if (bits_of_double_[pl] == 5)
	{
		if (bits_of_double_[pl] % 2 == 1)
		{
			bits_of_double_[--pl] ++;
		}
	}
	while (bits_of_double_[pl] >= 10)
	{
		bits_of_double_[pl] -= 10;
		bits_of_double_[--pl] ++;
	}
	if (get_sign(a) == -1) printf("-");
	printf("%d",bits_of_double_[st]);
	printf(".");
	for (int i = st + 1; i <= st + k; i++)
	{
		printf("%d",bits_of_double_[i]);
	}
	printf("e");
	if (HALF - st >= 0) printf("+");
	printf("%d\n", HALF - st);
}
char stack1[NUM_OF_CHAR];
struct double_n stack2[NUM_OF_CHAR];
void debug(int n1,int n2)
{
	for (int i = 1; i <= n1; i++)
		printf(" %c ",stack1[i]);
	printf("\n");
	for (int i = 1; i <= n2; i++)
		print_out_double_n(stack2[i], 10);
	printf("\n");
}
void getline_()
{
	int len = 0;
	while(1)
	{
		char c = getchar();
		if (c == '\n') break;
		str[len++] = c;
	}
}
void solve()
{
	memset(str, 0, sizeof(str));
	getline_();
	int last = -1, len = strlen(str), last_type = 0, pl;
	int num_of_s1 = 0, num_of_s2 = 0;
	for (pl = 0; pl < len; pl++)
	{
		if (str[pl] == ' ') continue;
		if (str[pl] == '.' || (str[pl] >= '0' && str[pl] <= '9') || (last_type == 0 && (str[pl] == '-' || str[pl] == '+') ) || str[pl] == 'i' || str[pl] == 'n')
		{
			if (str[pl] == '+') pl++;
			int sign = 1;
			if (str[pl] == '-')
			{
				sign = -1;
				pl++;
			}
			if (str[pl] == 'i' || str[pl] == 'n')
			{
				last_type = 1;
				if (str[pl] == 'i')
				{
					struct double_n inf;
					inf = change_sign(inf, sign);
					inf = change_exp(inf, bin[NUM_OF_EXP] - 1);
					inf = change_frac(inf, 0);
					stack2[++ num_of_s2] = inf;
					continue;
				}
				else
				{
					struct double_n nan;
					nan = change_sign(nan, sign);
					nan = change_exp(nan, bin[NUM_OF_EXP] - 1);
					nan = change_frac(nan, 1);
					stack2[++ num_of_s2] = nan;
					continue;
				}
			}
			last = pl;
			while ( (str[pl + 1] == '.' || (str[pl + 1] >= '0' && str[pl + 1] <= '9') || str[pl + 1] == 'e' || str[pl + 1] == 'E') && pl < len - 1)
			pl ++;
			struct double_n a = get_double_n(last, pl);
			a = change_sign(a, sign);
			stack2[++ num_of_s2] = a;
			last_type = 1;
		}
		else if (str[pl] == '+' || str[pl] == '-')
		{
			last_type = 0;
			while (stack1[num_of_s1] != '(' && num_of_s1 > 0)
			{
				struct double_n a, b, c;
				a = stack2[num_of_s2--];
				b = stack2[num_of_s2--];
				c = calculate(b, stack1[num_of_s1--], a);
				stack2[++num_of_s2] = c;
			}
			stack1[++num_of_s1] = str[pl];
		}
		else if (str[pl] == '*' || str[pl] == '/')
		{
			last_type = 0;
			while (stack1[num_of_s1] != '(' && stack1[num_of_s1] != '+' && stack1[num_of_s1] != '-' && num_of_s1 > 0)
			{
				struct double_n a, b, c;
				a = stack2[num_of_s2--];
				b = stack2[num_of_s2--];
				c = calculate(b, stack1[num_of_s1--], a);
				stack2[++num_of_s2] = c;
			}
			stack1[++num_of_s1] = str[pl];
		}
		else if (str[pl] == '(')
		{
			last_type = 0;
			stack1[++num_of_s1] = str[pl];
		}
		else if (str[pl] == ')')
		{
			while (stack1[num_of_s1] != '(' && num_of_s1 > 0)
			{
				struct double_n a, b, c;
				b = stack2[num_of_s2--];
				a = stack2[num_of_s2--];
				c = calculate(a, stack1[num_of_s1--], b);
				stack2[++num_of_s2] = c;
			}
			num_of_s1 --;
		}
	}
	while (num_of_s1 > 0)
	{
		struct double_n a, b, c;
		b = stack2[num_of_s2--];
		a = stack2[num_of_s2--];
		c = calculate(a, stack1[num_of_s1--], b);
		stack2[++num_of_s2] = c;
	}
	print_out_double_n(stack2[1], 40);
}

int main()
{
	freopen("data.txt","r",stdin);
	freopen("opcmd.txt","w",stdout);
	
	bin[0] = 1LL;
	for (int i = 1; i < NUM_OF_BIN; i++)
	bin[i] = bin[i -1] * 2;
	Bias = bin[NUM_OF_EXP - 1] - 1;
	get_pow_of_2();
	for (int k = 1; k <= 100; k++)
	{
		solve();
	}

	fclose(stdin);
	fclose(stdout);
	return 0;
}
