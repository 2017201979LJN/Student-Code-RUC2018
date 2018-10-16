#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#define NUM_OF_BYTES 8
#define swap(a,b) a ^= b ^= a ^=b
#define bigint __uint128_t

struct double_n{
    unsigned char bytes[NUM_OF_BYTES];
};

struct double_n read_input()
{
    struct double_n input_double;
    scanf("%lf", &input_double);
    return input_double;
}

void print_double (struct double_n v)
{
    double *output_double = &v;
    printf ("%.50le\n", *output_double);
}

void init_double (struct double_n *v)
{
    for (int i = 0; i < 8; i ++){
        v->bytes[i] = 0;
    }
}

unsigned int get_exp (struct double_n v)
{
    return ( ( ((unsigned int) v.bytes[7])  << 4)  + (v.bytes[6] >> 4) ) % (1 << 11);
}

int get_sign (struct double_n v)
{
    return v.bytes[7] >> 7 ? -1 : 1;
}

unsigned long long get_frac (struct double_n v)
{
    unsigned long long res = 0;
    for (int i = 7; i >= 0; i --){
        res += ( ( (unsigned long long) v.bytes[i]) << (8 * i) );
    }
    return res % (1ll << 52);
}

int is_inf (struct double_n v)
{
    if (get_exp (v) == 2047 && (!get_frac (v) ) )
        return 1;
    else
        return 0;
}

int is_nan (struct double_n v)
{
    if (get_exp (v) == 2047 && get_frac (v) )
        return 1;
    else 
        return 0;
}

int is_denor (struct double_n v)
{
    if (!get_exp (v) )
        return 1;
    else
        return 0;
}

int is_zero (struct double_n v)
{
    if (!get_exp (v) && !get_frac (v) )
        return 1;
    else
        return 0;
}

struct double_n cast_to_double (int sign, unsigned int exp, unsigned long long frac)
{
    struct double_n v;
    init_double (&v);
    if (sign == -1){
        v.bytes[7] |= (1 << 7);
    }
    v.bytes[7] += (exp >> 4);
    v.bytes[6] += ( (exp % (1 << 4)) << 4);
    for (int i = 0; i < 8; i ++){
        v.bytes[i] += frac % (1 << 8);
        frac = (frac >> 8);
    }
    return v;
}

unsigned long long rounding (int *exp, bigint frac, int len)
{
    long long e = frac >> len;
    while (!e && *exp){
        frac = (frac << 1);
        e = frac >> len;
        *exp = *exp - 1;
    }
    int is_de = 0;
    bigint frac_init = frac;
    int shift_cnt = 0;
    if (*exp + e < 0)
        shift_cnt ++;
    while (*exp + e < 0 && frac > 0){
        *exp = *exp + 1;
        frac = (frac >> 1);
        shift_cnt ++;
        e = frac >> len;
    }
    if (*exp < 0)
        return 0;
    if (e > 0){
        while (e > 1)
        {
            *exp = *exp + 1;
            frac = (frac >> 1);
            shift_cnt ++;
            e = frac >> len;
        }
    }
    else
       is_de = 1;
    int remain = len - 52 + shift_cnt;
    if (remain > 120)
        return 0;
    frac = frac_init;
    if (remain > 0)
    {
        bigint round_stand = ( (bigint) 1 << (remain - 1));
        bigint frac_remain = frac % ( (bigint) 1 << remain );
        if ( (frac_remain > round_stand) || ( (frac_remain == round_stand) && (frac & (1ll << remain) ) ) ){
            frac = frac >> remain;
            frac ++;
        }
        else
            frac = frac >> remain;
        e = frac >> 52;
        if (is_de && e)
            *exp = 1;
        if (e > 1)
        {
            *exp = *exp + 1;
            frac = (frac >> 1);
        }
    }
    return (unsigned long long) frac % (1ll << 52);
}

struct double_n add_same_sign (struct double_n v1, struct double_n v2)
{
    if ( is_nan(v1) )
        return v1;
    if ( is_nan(v2) )
        return v2;
    if ( is_inf(v1) )
        return v1;
    if ( is_inf(v2) )
        return v2;
    int sign1 = get_sign (v1), sign2 = get_sign (v2);
    unsigned int exp1 = get_exp (v1), exp2 = get_exp (v2);
    unsigned long long frac1 = get_frac (v1), frac2 = get_frac (v2);
    if (exp1 < exp2){
        swap(exp1, exp2);
        swap(frac1, frac2);
        swap(sign1, sign2);
    }
    int expdif = exp1 - exp2;
    if (expdif > 60){
        return cast_to_double (sign1, exp1, frac1 );
    }
    if ( !is_denor (v1) )
        frac1 += (1ll << 52);
    if ( !is_denor (v2) )
        frac2 += (1ll << 52);
    int exp = exp1;
    unsigned long long frac = rounding (&exp, ( (bigint) frac1 << expdif) + frac2, 52 + expdif);
    if ( exp >= 2047 ){
        exp = 2047;
        frac = 0;
    }
    return cast_to_double (sign1, exp, frac);
}

struct double_n add_dif_sign (struct double_n v1, struct double_n v2)
{
    if ( is_nan(v1) )
        return v1;
    if ( is_nan(v2) )
        return v2;
    if ( is_inf(v1) && is_inf(v2) && get_sign(v1) != get_sign(v2) ){
        v1.bytes[0] ++;
        return v1;
    }
    if ( is_inf(v1) )
        return v1;
    if ( is_inf(v2) )
        return v2;
    int sign1 = get_sign (v1), sign2 = get_sign (v2);
    unsigned int exp1 = get_exp (v1), exp2 = get_exp (v2);
    unsigned long long frac1 = get_frac (v1), frac2 = get_frac (v2);
    if ( ( (exp1 == exp2) && (frac1 < frac2) ) || exp1 < exp2 ){
        swap(exp1, exp2);
        swap(frac1, frac2);
        swap(sign1, sign2);
    }
    int expdif = exp1 - exp2;
    if (expdif > 60){
        return cast_to_double (sign1, exp1, frac1 );
    }
    if ( !is_denor (v1) )
        frac1 += (1ll << 52);
    if ( !is_denor (v2) )
        frac2 += (1ll << 52);
   int exp = exp2;
    unsigned long long frac = rounding (&exp, ( ( (bigint) frac1 ) << expdif ) - frac2, 52);
    if ( exp >= 2047 ){
        exp = 2047;
        frac = 0;
    }
    return cast_to_double (sign1, exp, frac);
}

struct double_n add (struct double_n v1, struct double_n v2)
{
    if ( get_sign (v1) == get_sign (v2) )
        return add_same_sign (v1, v2);
    return add_dif_sign (v1, v2);
}

struct double_n substract (struct double_n v1, struct double_n v2)
{
    int sign = v2.bytes[7] >> 7;
    v2.bytes[7] = v2.bytes[7] - (sign << 7) + ( (sign ^ 1) << 7);
    return add (v1, v2);
}

struct double_n multiply (struct double_n v1, struct double_n v2)
{
    if (is_nan (v1))
        return v1;
    if (is_nan (v2))
        return v2;
    if (is_inf (v1) && is_inf (v2) ){
        if (get_sign (v1) == get_sign (v2) )
            return cast_to_double (get_sign (v1) * get_sign (v2), get_exp (v1), get_exp (v1) );
        else{
            v1.bytes[0] ++;
            return v1;
        }
    }
    if ( (is_inf (v1) && is_zero (v2) ) || (is_zero (v1) && is_inf (v2) ) ){
        v1.bytes[0] ++;
        return v1;
    }
    if (is_inf (v1) )
        return cast_to_double (get_sign (v1) * get_sign (v2), get_exp (v1), get_exp (v1) );
    if (is_inf (v2) )
        return cast_to_double (get_sign (v1) * get_sign (v2), get_exp (v2), get_exp (v2) );
    int sign1 = get_sign (v1), sign2 = get_sign (v2);
    unsigned long long exp1 = get_exp (v1), exp2 = get_exp (v2);
    unsigned long long frac1 = get_frac (v1), frac2 = get_frac (v2);
    if (!is_denor (v1) )
        frac1 += (1ll << 52);
    if (!is_denor (v2) )
        frac2 += (1ll << 52);
    int sign_exp = (int) exp1 + (int) exp2 - 1023;
    unsigned long long frac = rounding (&sign_exp, ( (bigint) frac1) *frac2, 104);
    if ( sign_exp < 0)
        return cast_to_double (get_sign (v1) * get_sign (v2), 0, 0);
    unsigned int exp = sign_exp;
    if (exp >= (1ll << 11) - 1){
        exp = 2047;
        frac = 0;
    }
    return cast_to_double (sign1 * sign2, exp, frac);
}

struct double_n divide (struct double_n v1, struct double_n v2)
{
    if (is_nan (v1))
        return v1;
    if (is_nan (v2))
        return v2;
    if (is_zero (v1) && is_zero (v2))
        return cast_to_double (get_sign (v1) * get_sign (v2), 2047, 1);
    if (is_zero (v1))
        return cast_to_double (get_sign (v1) * get_sign (v2), 0, 0);
    if (is_zero (v2))
        return cast_to_double (get_sign (v1) * get_sign (v2), 2047, 0);
    if (is_inf (v1) && is_inf (v2) ){
        v1.bytes[0] ++;
        return v1;
    }
    if (is_inf (v1) )
        return cast_to_double (get_sign (v1) * get_sign (v2), get_exp (v1), get_exp (v1) );
    if (is_inf (v2) )
        return cast_to_double (get_sign (v1) * get_sign (v2), get_exp (v2), get_exp (v2) );
    int sign1 = get_sign (v1), sign2 = get_sign (v2);
    unsigned long long exp1 = get_exp (v1), exp2 = get_exp (v2);
    unsigned long long frac1 = get_frac (v1), frac2 = get_frac (v2);
    if (!is_denor (v1) )
        frac1 += (1ll << 52);
    if (!is_denor (v2) )
        frac2 += (1ll << 52);
    int sign_exp = (int) exp1 + 1023 - (int) exp2;
    unsigned long long frac;
    if (!frac2)
        frac = 0;
    else
        frac = rounding (&sign_exp, ( (bigint) frac1 << 64) / frac2, 64);
    if ( sign_exp < 0)
        return cast_to_double (get_sign (v1) * get_sign (v2), 0, 0);
    unsigned int exp = sign_exp;
     if (exp >= (1ll << 11) - 1){
        exp = 2047;
        frac = 0;
    }
    return cast_to_double (sign1 * sign2, exp, frac);
}

int main()
{
    return 0;
}
