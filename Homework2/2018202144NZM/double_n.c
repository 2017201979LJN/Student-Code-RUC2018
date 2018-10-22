#include <stdio.h>

#define NUM_OF_BYTES 8
#define NUM_OF_SIGN 1
#define NUM_OF_EXP 11
#define NUM_OF_FRAC 52
#define NUM_OF_BITS 8
#define BIGINT_SHIFT_LIM 120
#define EXP_DIF_LIM 60
#define DIVIDE_SHIFT_BIAS 110
#define bigint __uint128_t

#define swap(a,b) (a) ^= (b) ^= (a) ^= (b)

struct double_n{
    unsigned char bytes[NUM_OF_BYTES];
};

enum {
    ADD = '+',
    SUBSTRACT = '-',
    MULTIPLY = '*',
    DIVIDE = '/'
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
    for (int i = 0; i < NUM_OF_BYTES; i ++){
        v->bytes[i] = 0;
    }
}

int get_sign (struct double_n v)
{
    return v.bytes[NUM_OF_BYTES - 1] >> （NUM_OF_BITS - 1） ? -1 : 1;
}

unsigned int get_exp (struct double_n v)
{
    unsigned int res = 0;
    for (int i = 1; i <= NUM_OF_EXP; i ++){   
        int shift_index = i + NUM_OF_SIGN - 1;
        int bit_index = 7 - shift_index % NUM_OF_BITS, byte_index = NUM_OF_BYTES - shift_index / NUM_OF_BITS - 1;
        if (v.bytes[byte_index] & (1 << bit_index) ){
            res += (1 << (NUM_OF_EXP - i) );
        }
    }
    return res;
}

unsigned long long get_frac (struct double_n v)
{
    unsigned long long res = 0;
    for (int i = 1; i <= NUM_OF_FRAC; i ++){   
        int shift_index = i + NUM_OF_SIGN + NUM_OF_EXP - 1;
        int bit_index = 7 - shift_index % NUM_OF_BITS, byte_index = NUM_OF_BYTES - shift_index / NUM_OF_BITS - 1;
        if (v.bytes[byte_index] & (1 << bit_index) ){
            res += (1LL << (NUM_OF_FRAC - i) );
        }
    }
    return res;
}

int is_inf (struct double_n v)
{
    if (get_exp (v) == ( (1 << NUM_OF_EXP) - 1) && (!get_frac (v) ) )
        return 1;
    else
        return 0;
}

int is_nan (struct double_n v)
{
    if (get_exp (v) == ( (1 << NUM_OF_EXP) - 1) && get_frac (v) )
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
        v.bytes[NUM_OF_BYTES - 1] |= (1 << (NUM_OF_BITS - 1));
    }
    for (int i = NUM_OF_EXP; i >= 1; i --){
        int shift_index = i + NUM_OF_SIGN - 1;
        int bit_index = 7 - shift_index % NUM_OF_BITS, byte_index = NUM_OF_BYTES - shift_index / NUM_OF_BITS - 1;
        if (exp & 1)
            v.bytes[byte_index] +=  (1 << bit_index);
        exp = (exp >> 1);
    }
    for (int i = 0; i < NUM_OF_BYTES; i ++){
        v.bytes[i] += frac % (1 << NUM_OF_BITS);
        frac = (frac >> NUM_OF_BITS);
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
    int remain = len - NUM_OF_FRAC + shift_cnt;
    if (remain > BIGINT_SHIFT_LIM)
        return 0;
    frac = frac_init;
    if (remain > 0)
    {
        bigint round_stand = ( (bigint) 1 << (remain - 1));
        bigint frac_remain = frac % ( (bigint) 1 << remain );
        if ( (frac_remain > round_stand) || ( (frac_remain == round_stand) && (frac & (1LL << remain) ) ) ){
            frac = frac >> remain;
            frac ++;
        }
        else
            frac = frac >> remain;
        e = frac >> NUM_OF_FRAC;
        if (is_de && e)
            *exp = 1;
        if (e > 1)
        {
            *exp = *exp + 1;
            frac = (frac >> 1);
        }
    }
    return (unsigned long long) frac % (1LL << NUM_OF_FRAC);
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
    if (expdif > EXP_DIF_LIM){
        return cast_to_double (sign1, exp1, frac1 );
    }
    if ( !is_denor (v1) )
        frac1 += (1LL << NUM_OF_FRAC);
    if ( !is_denor (v2) )
        frac2 += (1LL << NUM_OF_FRAC);
    int exp = exp1;
    unsigned long long frac = rounding (&exp, ( (bigint) frac1 << expdif) + frac2, NUM_OF_FRAC + expdif);
    if ( exp >= ( (1 << NUM_OF_EXP) - 1) ){
        exp = ( (1 << NUM_OF_EXP) - 1);
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
    if (expdif > EXP_DIF_LIM){
        return cast_to_double (sign1, exp1, frac1 );
    }
    if ( !is_denor (v1) )
        frac1 += (1LL << NUM_OF_FRAC);
    if ( !is_denor (v2) )
        frac2 += (1LL << NUM_OF_FRAC);
   int exp = exp2;
    unsigned long long frac = rounding (&exp, ( ( (bigint) frac1 ) << expdif ) - frac2, NUM_OF_FRAC);
    if ( exp >= ( (1 << NUM_OF_EXP) - 1) ){
        exp = ( (1 << NUM_OF_EXP) - 1);
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
    int sign = v2.bytes[NUM_OF_BYTES - 1] >> (NUM_OF_BITS - 1);
    v2.bytes[NUM_OF_BYTES - 1] = v2.bytes[NUM_OF_BYTES - 1] - (sign << (NUM_OF_BITS - 1)) + ( (sign ^ 1) << (NUM_OF_BITS - 1));
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
        frac1 += (1LL << NUM_OF_FRAC);
    if (!is_denor (v2) )
        frac2 += (1LL << NUM_OF_FRAC);
    int sign_exp = (int) exp1 + (int) exp2 - ( (1 << (NUM_OF_EXP - 1) ) - 1);
    if ( (!exp1 || !exp2) && sign_exp > 0 )
        sign_exp ++;
    unsigned long long frac = rounding (&sign_exp, ( (bigint) frac1) *frac2, NUM_OF_FRAC * 2);
    if ( sign_exp < 0)
        return cast_to_double (get_sign (v1) * get_sign (v2), 0, 0);
    unsigned int exp = sign_exp;
    if (exp >= (1LL << NUM_OF_EXP) - 1){
        exp = ( (1 << NUM_OF_EXP) - 1);
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
        return cast_to_double (get_sign (v1) * get_sign (v2), ( (1 << NUM_OF_EXP) - 1), 1);
    if (is_zero (v1))
        return cast_to_double (get_sign (v1) * get_sign (v2), 0, 0);
    if (is_zero (v2))
        return cast_to_double (get_sign (v1) * get_sign (v2), ( (1 << NUM_OF_EXP) - 1), 0);
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
        frac1 += (1LL << NUM_OF_FRAC);
    if (!is_denor (v2) )
        frac2 += (1LL << NUM_OF_FRAC);
    int sign_exp = (int) exp1 + ( (1 << (NUM_OF_EXP - 1) ) - 1) - (int) exp2;
    if (!exp1 && sign_exp > 0)
        sign_exp ++;
    if (!exp2 && sign_exp > 0)
        sign_exp --;
    unsigned long long frac;
    int fir_bit = NUM_OF_FRAC + 1;
    for (int i = NUM_OF_FRAC; i >= 0; i --){
        if (frac1 & (1LL << i) ){
            fir_bit = i + 1;
            break;
        }
    }
    if (!frac2)
        frac = 0;
    else
        frac = rounding (&sign_exp, ( (bigint) frac1 << (DIVIDE_SHIFT_BIAS - fir_bit) ) / frac2, (DIVIDE_SHIFT_BIAS - fir_bit) );
    if ( sign_exp < 0)
        return cast_to_double (get_sign (v1) * get_sign (v2), 0, 0);
    unsigned int exp = sign_exp;
     if (exp >= (1LL << NUM_OF_EXP) - 1){
        exp = ( (1 << NUM_OF_EXP) - 1);
        frac = 0;
    }
    return cast_to_double (sign1 * sign2, exp, frac);
}

int main()
{
    freopen ("double_n.in", "r", stdin);
    freopen ("double_n.out", "w", stdout);
    struct double_n a = read_input(), b;
    char op[2];
    scanf ("%s", op);
    b = read_input();
    struct double_n c;
    if (op[0] == ADD)
        c = add (a, b);
    else if (op[0] == SUBSTRACT)
        c = substract (a, b);
    else if (op[0] == MULTIPLY)
        c = multiply (a, b);
    else if (op[0] == DIVIDE)
        c = divide (a, b);
    print_double (c);
    return 0;
}
