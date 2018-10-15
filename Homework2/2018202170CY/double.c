#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define bool char
#define NUM_OF_TWO_POW 5000
#define NUM_OF_BYTES 8
#define NUM_OF_BIN 63
#define NUM_OF_FRAC 52
#define NUM_OF_EXP 11
struct double_n
{
	unsigned char bytes[NUM_OF_BYTES];
};
long long bin[NUM_OF_BIN];
int Bias;
void print_bits_of_double_n(struct double_n a)
{
	for (int i = 0; i < NUM_OF_BYTES; i++)
	{
		for (int j = 7; j >=0; j--)
		{
			printf("%lld", (a.bytes[i] & bin[j]) >> j);
		}
		printf(" ");
	}
	printf("\n");
}
void swap(struct double_n *a, struct double_n *b)
{
	struct double_n c = *b;
	*b = *a;
	*a = c;
}
int get_exp(struct double_n a)
{
	int exp = 0;
	exp = a.bytes[0] & (bin[7] - 1);
	exp <<= 4;
	exp |= ( a.bytes[1] & ( (bin[8] - 1) - (bin[4] - 1) ) ) >> 4;
	return exp;
}
long long get_frac(struct double_n a)
{
	long long frac = a.bytes[1] & (bin[4] - 1);
	for (int i = 2; i < NUM_OF_BYTES; i++)
	{
		frac <<= 8;
		frac |= a.bytes[i];
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
	return get_exp(a) == 0 && get_frac(a) == 0;
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
//	printf("%d %d\n", get_exp(a) , Bias);
	return check_normalize(a) ? (get_exp(a) - Bias) : (1 - Bias);
}
double get_significand(struct double_n a)
{
	long long frac_of_a = get_frac(a);
	//printf("%lld ", frac_of_a);
	double pow = 0.5, significand = 0.0;
	for (int i = NUM_OF_FRAC - 1; i >= 0; i--)
	{
		if (frac_of_a & bin[i])
		{
			significand += pow;
		}
		pow /= 2.0;
	}
	int exp_of_a = get_exp(a) ;
	return exp_of_a == 0 ? significand : significand + 1.0;
}
int get_sign(struct double_n a)
{
	if(a.bytes[0] & bin[7]) return -1;
	return 1;
}
struct double_n change_sign(struct double_n a,int sign_of_a)
{
	if (sign_of_a == 1) a.bytes[0] &= bin[NUM_OF_BYTES - 1] - 1;
	else a.bytes[0] |= bin[NUM_OF_BYTES - 1];	
	return a;
}
struct double_n change_exp(struct double_n a,int exp_of_a)
{
	struct double_n b = a;
	b.bytes[0] &= bin[7] ;
	b.bytes[0] |= ( exp_of_a & ( (bin[NUM_OF_EXP] - 1) - (bin[4] - 1) ) ) >> 4;
	b.bytes[1] &= bin[4] - 1;
	b.bytes[1] |= (exp_of_a & (bin[4] - 1) ) << 4 ;
	return b;
}
struct double_n change_frac(struct double_n a,long long frac_of_a)
{
	struct double_n b = a;
	b.bytes[1] &= (bin[8] - 1) - (bin[4] - 1);
	b.bytes[1] |= ( frac_of_a & ( (bin[52] - 1) - (bin[48] - 1) ) ) >> 48 ;
	for (int i = 2; i < NUM_OF_BYTES - 1; i++)
	{
		b.bytes[i] = ( frac_of_a & ( (bin[ (NUM_OF_BYTES - i) * 8 ] - 1) - ( bin[ (NUM_OF_BYTES - i - 1) * 8 ] - 1) ) ) >> ( (NUM_OF_BYTES - i - 1) * 8);
	}
	b.bytes[NUM_OF_BYTES - 1] = frac_of_a & (bin[8] - 1) ;
	return b;
}
struct double_n right_shift(struct double_n a)
{
	struct double_n b;
	int sign_of_b = get_sign(a);
	int exp_of_b = get_exp(a) + 1;
	long long frac_of_b = get_frac(a) / 2;
	b = change_sign(b, sign_of_b);
	b = change_exp(b, exp_of_b);
	b = change_frac(b, frac_of_b);
	return b;
}
double two_pow[NUM_OF_TWO_POW];
double pow_of_two(int a)
{
	return two_pow[a + Bias - 1];
}
bool compare(struct double_n a,struct double_n b)
{
	int exp_of_a = get_exp(a), exp_of_b = get_exp(b);
	if( exp_of_a != exp_of_b ) return exp_of_a < exp_of_b ;
	long long frac_of_a = get_frac(a), frac_of_b = get_frac(b);
	return frac_of_a < frac_of_b ;
}
struct double_n plus(struct double_n a, struct double_n b)
{
	if ( get_sign(a) == get_sign(b) )
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
				b = right_shift(b) ;
				exp_of_b ++;
				long long frac_of_b = get_frac(b) + bin[NUM_OF_FRAC - 1];
				b = change_frac(b, frac_of_b);
				while( ( exp_of_b < exp_of_a ) && ( check_frac(b) ) )
				{
					b = right_shift(b);
					exp_of_b ++;
				}
				if(exp_of_b == exp_of_a)
				{
					long long frac_of_a = get_frac(a), frac_of_b = get_frac(b);
					if(frac_of_a + frac_of_b >= bin[NUM_OF_FRAC] )
					{
						int exp_of_c = exp_of_a + 1;
						long long frac_of_c = (frac_of_a + frac_of_b - bin[NUM_OF_FRAC] ) / 2 ;
						c = change_exp(c , exp_of_c);
						c = change_frac(c , frac_of_c);
						return c;
					}
					else
					{
						int exp_of_c = exp_of_a ;
						long long frac_of_c = frac_of_a + frac_of_b ;
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
			while (exp_of_b < exp_of_a && check_frac(b) )
			{
				b = right_shift(b);
				exp_of_b ++ ;
			}
			if (exp_of_b == exp_of_a)
			{
				long long frac_of_a = get_frac(a), frac_of_b = get_frac(b);
				if(frac_of_a + frac_of_b >= bin[NUM_OF_FRAC] )
				{
					int exp_of_c = exp_of_a + 1;
					long long frac_of_c = (frac_of_a + frac_of_b - bin[NUM_OF_FRAC] ) / 2 ;
					c = change_exp(c , exp_of_c);
					c = change_frac(c , frac_of_c);
					return c;
				}
				else
				{
					int exp_of_c = exp_of_a ;
					long long frac_of_c = frac_of_a + frac_of_b ;
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
	else
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
			b = right_shift(b);
			exp_of_b ++;
			b = change_frac(b, get_frac(b) + bin[NUM_OF_FRAC - 1]);
			while (exp_of_b < exp_of_a && check_frac(b) )
			{
				b = right_shift(b);
				exp_of_b ++;
			}
			if (!check_frac(b) ) return a;
			long long frac_of_a = get_frac(a), frac_of_b = get_frac(b);
			if (frac_of_a < frac_of_b)
			{
				long long frac_of_c = frac_of_a - frac_of_b + bin[NUM_OF_FRAC];
				int exp_of_c = exp_of_a;
				while (frac_of_c < bin[NUM_OF_FRAC])
				{
					frac_of_c *= 2;
					exp_of_c --;
				}
				if (exp_of_c == 0)
				{
					c = change_exp(c, exp_of_c);
					c = change_frac(c, frac_of_c);
					return c;
				}
				frac_of_c -= bin[NUM_OF_FRAC];
				c = change_exp(c, exp_of_c);
				c = change_frac(c, frac_of_c);
				return c;
			}
			else
			{
				long long frac_of_c = frac_of_a - frac_of_b;
				int exp_of_c = exp_of_a;
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
			while (exp_of_a > exp_of_b && check_frac(b) );
			{
				b = right_shift(b);
				exp_of_b ++;
			}
			if (!check_frac(b) ) return a;
			long long frac_of_a = get_frac(a), frac_of_b = get_frac(b);
			if (frac_of_a < frac_of_b)
			{
				long long frac_of_c = frac_of_a - frac_of_b + bin[NUM_OF_FRAC];
				int exp_of_c = exp_of_a;
				while (frac_of_c < bin[NUM_OF_FRAC])
				{
					frac_of_c *= 2;
					exp_of_c --;
				}
				if (exp_of_c == 0)
				{
					c = change_exp(c, exp_of_c);
					c = change_frac(c, frac_of_c);
					return c;
				}
				frac_of_c -= bin[NUM_OF_FRAC];
				c = change_exp(c, exp_of_c);
				c = change_frac(c, frac_of_c);
				return c;
			}
			else
			{
				long long frac_of_c = frac_of_a - frac_of_b;
				int exp_of_c = exp_of_a;
				c = change_exp(c, exp_of_c);
				c = change_frac(c, frac_of_c);
				return c;
			}
						
		}
	}
}
struct double_n minus(struct double_n a, struct double_n b)
{
	b = change_sign(b, get_sign(b) * (-1) );
	return plus(a, b);
}
int bit[3][NUM_OF_FRAC * 10];
long long get_frac_of_c(long long frac_of_a , long long frac_of_b )
{
	for (int i = 1; i <= NUM_OF_FRAC; i++)
	bit[0][NUM_OF_FRAC + 1 - i] = (bin[i - 1] & frac_of_a) ? 1 : 0;
	for (int i = 1; i <= NUM_OF_FRAC; i++)
	bit[1][NUM_OF_FRAC + 1 - i] = (bin[i - 1] & frac_of_b) ? 1 : 0;
	for (int i = 1; i <= NUM_OF_FRAC * 2; i++)
	bit[2][i] = 0;
	for (int i = 1; i <= NUM_OF_FRAC; i++)
	{
		for (int j = 1; j <= NUM_OF_FRAC; j++)
		{
			bit[2][i + j] += bit[0][i] * bit[1][j];
		}
	}
	for (int i = NUM_OF_FRAC * 2; i >= 1; i--)
	{
		bit[2][i - 1] += bit[2][i] / 2;
		bit[2][i] %= 2;
	}
	long long frac_of_c = 0;
	for (int i = NUM_OF_FRAC; i >= 1; i--)
	{
		frac_of_c += bin[NUM_OF_FRAC - i] * bit[2][i] ;
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
		if (exp_of_c < 0)
		{
			c = change_sign(c, sign_of_c);
			c = change_exp(c, 0);
			c = change_frac(c, 0);
			return c;
		}
		long long frac_of_c = get_frac_of_c(frac_of_a, frac_of_b) + bin[NUM_OF_FRAC] + frac_of_a + frac_of_b;
		if (frac_of_c >= bin[NUM_OF_FRAC + 1]) frac_of_c /= 2, exp_of_c ++ ;
		if (exp_of_c >= bin[NUM_OF_EXP]) exp_of_c = bin[NUM_OF_EXP] - 1, frac_of_c = 1;
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
		c = change_exp(c, 0);
		return c;
	}
	if (check_denormalize(a) || check_denormalize(b) )
	{
		if (check_denormalize(a) ) swap(&a, &b);
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
		long long frac_of_c = get_frac_of_c(frac_of_b, frac_of_a) + frac_of_b;
		if (frac_of_c >= bin[NUM_OF_FRAC + 1]) frac_of_c /= 2, exp_of_c ++ ;
		while (frac_of_c < bin[NUM_OF_FRAC])
		{
			exp_of_c --, frac_of_c *= 2;
			if (frac_of_c >= bin[NUM_OF_FRAC])
			{
				frac_of_c -= bin[NUM_OF_FRAC];
				break;
			}
		}
		if (exp_of_c >= bin[NUM_OF_EXP]) exp_of_c = bin[NUM_OF_EXP] - 1, frac_of_c = 1;
		c = change_exp(c, exp_of_c);
		c = change_frac(c, frac_of_c);
		//printf("c: %d %.8lf\n",get_exponent(c), get_significand(c));
		return c;
	}
}
int bits_of_frac_of_c[NUM_OF_FRAC * 10];
long long get_frac_of_double_n_c(long long feng_zi, long long feng_mu)
{
	for (int i = 1; i <= NUM_OF_FRAC; i++)
	{
		feng_zi *= 2;
		if (feng_zi >= feng_mu)
		{
			bits_of_frac_of_c[i] = 1;
			feng_zi -= feng_mu;
		}
		else
		{
			bits_of_frac_of_c[i] = 0;
		}
	}
	long long frac_of_c = 0LL;
	for (int i = 1; i <= NUM_OF_FRAC; i++)
	{
		frac_of_c += bits_of_frac_of_c[i] ? bin[NUM_OF_FRAC - i] : 0;
	}
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
double print_out_double_n(struct double_n a)
{	
	double sign_of_a = get_sign(a), exponent_of_a = get_exponent(a), significand_of_a = get_significand(a);
	if ( check_inf(a) )
	{
		if ( sign_of_a == -1 ) printf("-");
		printf("inf\n");
		return 0;
	}
	if ( check_nan(a) )
	{
		if ( sign_of_a == -1) printf("-");
		printf("nan\n");
		return 0;
	}
	double answer = sign_of_a * pow_of_two( get_exponent(a) ) * get_significand(a);
	return answer;
}
struct double_n change_double_to_double_n(double a)
{
	struct double_n b;
	int sign_of_b = (a > 0) ? 1 : -1;
	int exp_of_b = Bias;
	while ( a >= 2 )
	{
		exp_of_b ++;
		a /= 2;
	}
	long long frac_of_b = 0LL;
	if( a >= 1) a -= 1;
	double pow = 0.5;
	for (int i = NUM_OF_FRAC; i >= 1; i--)
	{
		if (a >= pow)
		{
			frac_of_b += bin[i - 1];
			a -= pow;
		}
		pow /= 2;
	}
	b = change_sign(b, sign_of_b);
	b = change_exp(b, exp_of_b);
	b = change_frac(b, frac_of_b);
	return b;
}
int bit_of_a[NUM_OF_BYTES * 8];
struct double_n rounding(struct double_n a, int k)
{
	struct double_n b = a;
	long long frac_of_b = get_frac(a);
	int exp_of_b = get_exp(a);
	long long value = frac_of_b & (bin[NUM_OF_FRAC - k] - 1);
	if (value != bin[NUM_OF_FRAC - k - 1])
	{
		if (value < bin[NUM_OF_FRAC - k - 1])
		{
			frac_of_b &= bin[NUM_OF_BIN - 1] - bin[NUM_OF_FRAC - k];
		}
		else
		{
			frac_of_b &= bin[NUM_OF_BIN - 1] - bin[NUM_OF_FRAC - k];
			frac_of_b += bin[NUM_OF_FRAC - k];
		}
	}
	else
	{
		if (frac_of_b & bin[NUM_OF_FRAC - k])
		{
			frac_of_b &= bin[NUM_OF_BIN - 1] - bin[NUM_OF_FRAC - k];
			frac_of_b += bin[NUM_OF_FRAC - k];
		}
		else
		{
			frac_of_b &= bin[NUM_OF_BIN - 1] - bin[NUM_OF_FRAC - k];
		}
	}
	while (frac_of_b >= bin[NUM_OF_FRAC])
	{
		frac_of_b /= 2;
		exp_of_b ++;
	}
	b = change_exp(b, exp_of_b);
	b = change_frac(b, frac_of_b);
	return b;
}
void out(double a)
{
	printf("%lf\n", a);
}
double x[10];
char y[4] = {'+','-','*','/'};
double run(double a, char op, double b)
{
	if (op == '+') return a + b;
	if (op == '-') return a - b;
	if (op == '*') return a * b;
	if (op == '/') return a / b;
}
void print_table()
{
	for (int i = 0; i <= 7; i++)
	for (int j = 0; j <= 7; j++)
	{
		for (int k = 0; k < 4; k++)
		printf("%lf %c %lf == %lf\n", x[i], y[k], x[j], run(x[i],y[k],x[j]));
	}
}
#define NUM_OF_TEST 10010
double my_answer[NUM_OF_TEST],std_answer[NUM_OF_TEST];
int main()
{
	srand( time(0) );
	bin[0] = 1LL;
	for (int i = 1; i < NUM_OF_BIN; i++)
	{
		bin[i] = bin[i - 1] << 1;	
	}
	Bias = bin[NUM_OF_EXP - 1] - 1;
	two_pow[ Bias - 1 ] = 1.0;
	for (int i = Bias; i <= bin[NUM_OF_EXP] - 1; i++)
	two_pow[i] = two_pow[i - 1] * 2.0;
	for (int i = Bias - 1 ; i >= 0; i--)
	two_pow[i] = two_pow[i + 1] / 2.0;
	/*double inf = 1.0 / 0.0, nan = inf / inf;
	x[0] = inf; x[1] = -1 * inf; x[2] = -1 * nan; x[3] = nan; x[4] = 0.0; x[5] = -0.0; x[6] = 1.0; x[7] = -1.0;
	for(int i = 0; i <= 7; i++)
	printf("%lf ", x[i]);
	printf("\n");
	print_table();*/
	int exp = rand() % 255;
	for (int i = 1; i <= NUM_OF_TEST; i++)
	{
		double f1 = rand() + (double)rand() / rand(), f2 = rand() + (double)rand() / rand();
		char op = y[rand() % 4];
		struct double_n a = change_double_to_double_n(f1) , b = change_double_to_double_n(f2);
		struct double_n c = calculate(a , op , b);
		my_answer[i] = print_out_double_n(c); 
		std_answer[i] = run(f1, op, f2);
	}
	FILE *fp;
	fp = fopen("my_answer.txt","w+");
	for (int i = 1; i <= NUM_OF_TEST; i++)
	{
		fprintf(fp,"%.32e\n",my_answer[i]);
	}
	fclose(fp);
	fp = fopen("std_answer.txt","w+");
	for (int i = 1; i <= NUM_OF_TEST; i++)
	{
		fprintf(fp,"%.32e\n",std_answer[i]);
	}
	fclose(fp);
	return 0;
}
