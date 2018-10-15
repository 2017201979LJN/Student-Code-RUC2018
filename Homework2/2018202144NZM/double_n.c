#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define swap(a,b) a ^= b ^= a ^=b
#define bigint __uint128_t

const unsigned int bias = 1023;

struct double_n{
    unsigned char bytes[8];
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
    printf ("%.30lf\n", *output_double);
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

unsigned long long shift_right (unsigned long long v, int len)
{
    unsigned long long remain = v % (1ll << len);
    unsigned long long round_stand = (1ll << (len - 1));
    if (remain > round_stand)
        return (v >> len) + 1;
    else if (remain < round_stand)
        return v >> len;
    else 
    {
        if (v & (1ll << len))
            return (v >> len) + 1;
        else 
            return v >> len;
    }
}


unsigned long long rounding (unsigned int *exp, bigint frac, int len)
{
    unsigned int e = frac >> len;
    while (!e && *exp){
        frac = (frac << 1);
        e = frac >> len;
        *exp = *exp - 1;
    }
    int is_de = 0;
    bigint frac_init = frac;
    int shift_cnt = 0;
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
    frac = frac_init;
    if (remain > 0)
    {
        bigint round_stand = (1ll << (remain - 1));
        bigint frac_remain = frac % (1ll << remain);
        if ( (frac_remain > round_stand) || ( (frac_remain == round_stand) && (frac & (1ll << remain) ) ) ){
            if (is_de)
                e = 1;
            frac = frac >> remain;
            frac ++;
        }
        else
            frac = frac >> remain;
        e = frac >> len;
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
    if (expdif > 52){
        return cast_to_double (sign1, exp1, frac1 );
    }
    if ( !is_denor (v1) )
        frac1 += (1ll << 52);
    if ( !is_denor (v2) )
        frac2 += (1ll << 52);
    unsigned int exp = exp1;
    unsigned long long frac = rounding (&exp, ( (bigint) frac1 << expdif) + frac2, 52 + expdif);
    /*frac2 = shift_right (frac2, expdif);
    unsigned long long frac = frac1 + frac2;
    int exp = (frac >> 52) + exp1 - 1;
    frac = frac % (1ll << 52);*/
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
    if (expdif > 52){
        return cast_to_double (sign1, exp1, frac1 );
    }
    if ( !is_denor (v1) )
        frac1 += (1ll << 52);
    if ( !is_denor (v2) )
        frac2 += (1ll << 52);
    unsigned int exp = exp2;
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

print_bigint (bigint v)
{
    long long v1 = (v >> 64);
    long long v2 = v;
    print_2_base (v1, 64);
    print_2_base (v2, 64);
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
    unsigned int exp = exp1 + exp2 - 1023;
    unsigned long long frac = rounding (&exp, ( (bigint) frac1) *frac2, 104);
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
        return cast_to_double (get_sign (v1) * get_sign (v2), 0, 1);
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
    unsigned int exp = exp1 - exp2 + 1023;
    if (exp < 0)
        return cast_to_double (sign1 * sign2, 0, 0);
    unsigned long long frac;
    if (!frac2)
        frac = 0;
    else
        frac = rounding (&exp, ( (bigint) frac1 << 64) / frac2, 64);
     if (exp >= (1ll << 11) - 1){
        exp = 2047;
        frac = 0;
    }
    return cast_to_double (sign1 * sign2, exp, frac);
}

void print_2_base (unsigned long long v, int len)
{
    int s[100];
    int top = 0;
    while (v){
        top ++;
        if (v & 1)
            s[top] = 1;
        else
            s[top] = 0;
        v = (v >> 1);
    }
    for (int i = len; i > top; i--)
        printf("0");
    while (top){
        printf("%d", s[top]);
        top --;
    }
    printf("\n");
}

void print_double_2_base (struct double_n v)
{
    printf("%d\n", get_sign (v) );
    print_2_base (get_exp (v), 11);
    print_2_base (get_frac (v), 52);
}

int main()
{
    freopen("double_n.in", "r", stdin);
    freopen("double_n.out","w",stdout);
    struct double_n a=read_input(), b=read_input();
    struct double_n c = add (a,b);
    print_double (c);
    c = multiply (a,b);
    print_double (c);
    c = divide (a,b);
    print_double (c);
    return 0;
}
