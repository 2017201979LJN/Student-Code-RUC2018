#include <stdio.h>
#include <string.h>

#define NUM_OF_BYTES 8
#define NUM_OF_SIGN 1
#define NUM_OF_EXP 11
#define NUM_OF_FRAC 52
#define NUM_OF_BITS 8
#define BIGINT_SHIFT_LIM 120
#define EXP_DIF_LIM 60
#define DIVIDE_SHIFT_BIAS 110
#define MAX_OPER_LEN 100000
#define NUM_CALC_BASE 10
#define BUF_SIZE 100000
#define CH_SIZE 300
#define CONST_DBL_NUM 11
#define bigint __uint128_t

#define HIGH_ACCUR_SIZE 1000

#define swap(a,b) (a) ^= (b) ^= (a) ^= (b)

struct double_n{
    unsigned char bytes[NUM_OF_BYTES];
};

struct high_accur{
    char bits[HIGH_ACCUR_SIZE];
};

enum {
    ADD = '+',
    SUBSTRACT = '-',
    MULTIPLY = '*',
    DIVIDE = '/',
    LEFT_BRACK = '(',
    RIGHT_BRACK = ')'
};

enum {
    CH_ZERO = '0',
    CH_NINE = '9',
    CH_ENTER = '\n'
};

void init_double (struct double_n *v)
{
    for (int i = 0; i < NUM_OF_BYTES; i ++){
        v->bytes[i] = 0;
    }
}

int get_sign (struct double_n v)
{
    return v.bytes[NUM_OF_BYTES - 1] >> NUM_OF_BITS ? -1 : 1;
}

unsigned int get_exp (struct double_n v)
{
    return ( ( ((unsigned int) v.bytes[7])  << 4)  + (v.bytes[6] >> 4) ) % (1 << 11);
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

struct double_n CONST_DBL[CONST_DBL_NUM];

void init_const_dbl()
{
    CONST_DBL[0] = cast_to_double (0, 0, 0);
    CONST_DBL[1] = cast_to_double (0, ( (1 << (NUM_OF_EXP - 1) ) - 1), 0);
    for (int i = 2; i < CONST_DBL_NUM; i++) {
        CONST_DBL[i] = add (CONST_DBL[i-1], CONST_DBL[1]);
    }
}

struct double_n double_pow (struct double_n v, int exp)
{
    struct double_n result = CONST_DBL[1];
    for (int i = 1; i <= exp; i++) {
        result = multiply (result, v);
    }
    return result;
}

char buf[BUF_SIZE];
int buf_top=0;

char get_ch()
{
    return (buf_top > 0) ? buf[--buf_top] : getchar();
}

void push_ch (char ch)
{
    buf[buf_top++] = ch;
}

struct double_n read_double()
{
    init_const_dbl();
    struct double_n digit_part = CONST_DBL[0];
    char ch;
    for (ch = get_ch(); ch >= CH_ZERO && ch <= CH_NINE ; ch = get_ch() ) {
        digit_part = add (multiply (digit_part, CONST_DBL[10]), CONST_DBL[ch - CH_ZERO]);
    }
    if (ch == '.') {
        struct double_n float_part = CONST_DBL[0];
        int float_shift = 0;
        for (ch = get_ch(); ch >= CH_ZERO && ch <= CH_NINE; ch = get_ch() ) {
            float_shift++;
            float_part = add (multiply (float_part, CONST_DBL[10]), CONST_DBL[ch - CH_ZERO]);
        }
        struct double_n float_base = divide (CONST_DBL[1], double_pow (CONST_DBL[10], float_shift) );
        digit_part = add (digit_part, multiply(float_part, float_base) );
    }
    if (ch == 'e' || ch == 'E') {
        int exp = 0, sign = 1;
        ch = get_ch();
        if (ch == '-') {
            sign = -1;
        }
        else {
            push_ch (ch);
        }
        for (ch = get_ch(); ch >= CH_ZERO && ch <= CH_NINE; ch = get_ch() ) {
            exp = exp * 10 + ch - CH_ZERO;
        }
        struct double_n exp_base = double_pow (CONST_DBL[10], exp);
        if (sign == -1)
            exp_base = divide (CONST_DBL[1], exp_base);
        digit_part = multiply (digit_part, exp_base);
    }
    push_ch (ch);
    return digit_part;
}

int get_oper_type (char ch)
{
    if (ch == CH_ENTER) return -1;
    if (ch >= CH_ZERO && ch <= CH_NINE)
        return 1;
    if (ch == ADD || ch == SUBSTRACT || ch == MULTIPLY || ch == DIVIDE || ch == LEFT_BRACK)
        return 2;
    if (ch == RIGHT_BRACK)
        return 3;
    return 0;
}

void init_ch_map (int *ch_map)
{
    ch_map['+'] = ch_map['-'] = 0;
    ch_map['*'] = ch_map['/'] = 1;
    ch_map['('] = 2;
}

int post_exp_tot;

void get_post_expression (int *post_expression, struct double_n *double_list)
{
    char ch;
    int oper_type;
    int oper_stack[MAX_OPER_LEN], oper_s_top = 0, list_size = 0;
    int ch_map[CH_SIZE];
    post_exp_tot = 0;
    init_ch_map (ch_map);
    for (ch = get_ch(); ; ch = get_ch() ) {
        oper_type = get_oper_type (ch);
        if (oper_type == -1 )
            break;
        switch (oper_type) {
            case 1:
                push_ch (ch);
                double_list[list_size++] = read_double ();
                post_expression[post_exp_tot++] = list_size - 1;
                break;
            case 2:
                while (oper_s_top) {
                    int top_ch = oper_stack[oper_s_top - 1];
                    if (ch_map[ch] <= ch_map[top_ch] && top_ch != '(') {
                        post_expression[post_exp_tot++] = oper_stack[--oper_s_top] + MAX_OPER_LEN;
                    }
                    else 
                        break;
                }
                oper_stack[oper_s_top++] = ch;
                break;
            case 3:
                while (oper_s_top){
                    int top_ch = oper_stack[--oper_s_top];
                    if (top_ch == '(')
                        break;
                    post_expression[post_exp_tot++] = top_ch + MAX_OPER_LEN;
                }
                break;
            default:
                printf("invalid\n");
                break;
        }
    }
    while (oper_s_top) {
        post_expression[post_exp_tot++] = oper_stack[--oper_s_top] + MAX_OPER_LEN;
    }
}

struct double_n calc (struct double_n v1, struct double_n v2, char oper)
{
    switch (oper) {
        case ADD:
            return add (v1, v2);
            break;
        case SUBSTRACT:
            return substract (v1, v2);
            break;
        case MULTIPLY:
            return multiply (v1, v2);
            break;
        case DIVIDE:
            return divide (v1, v2);
            break;
        default:
            break;
    }
}

struct double_n calc_post_expression (int *post_expression, struct double_n *double_list)
{
    int oper_stack[MAX_OPER_LEN], oper_s_tot = 0;
    for (int i = 0; i < post_exp_tot; i++) {
        int top_ch = post_expression[i];
        if (top_ch >= MAX_OPER_LEN) {
            top_ch -= MAX_OPER_LEN;
            int top_num = oper_stack[--oper_s_tot], second_num = oper_stack[--oper_s_tot];
            double_list[top_num] = calc (double_list[second_num], double_list[top_num], top_ch);
            oper_stack[oper_s_tot++] = top_num;
        }
        else
            oper_stack[oper_s_tot++] = top_ch;
    }
    return double_list[oper_stack[0]];
}

void init_accur (struct high_accur *v)
{
    for (int i = 0; i < HIGH_ACCUR_SIZE; i++) {
        v->bits[i] = 0;
    }
}

struct high_accur h_add (struct high_accur v1, struct high_accur v2)
{
    int last_res = 0;
    for (int i = 0; i < HIGH_ACCUR_SIZE; i++) {
        int sum = v1.bits[i] + v2.bits[i] + last_res;
        v1.bits[i] = sum % 10;
        last_res = sum / 10;
    }
    return v1;
}

struct high_accur mult_2 (struct high_accur v)
{
    int last_res = 0;
    for (int i = 0; i < HIGH_ACCUR_SIZE; i++) {
        int mult = v.bits[i] * 2 + last_res;
        v.bits[i] = mult % 10;
        last_res = mult / 10;
    }
    return v;
}

struct high_accur div_2 (struct high_accur v)
{
    int last_res = 0;
    for (int i = HIGH_ACCUR_SIZE; i >= 0 ;i--) {
        int cur_bit = v.bits[i];
        v.bits[i] = (v.bits[i] + last_res * 10) / 2;
        last_res = (cur_bit + last_res * 10) & 1;
    }
    return v;
}

void print_accur (struct high_accur v, int len)
{
    struct high_accur round_in;
    init_accur (&round_in);
    round_in.bits[HIGH_ACCUR_SIZE / 2 - len] = 1;
    if (v.bits[HIGH_ACCUR_SIZE / 2 - len - 1] > 5)
        v = h_add (v, round_in);
    int is_zero = 1;
    for (int i = HIGH_ACCUR_SIZE - 1; i >= HIGH_ACCUR_SIZE / 2; i--) {
        if (v.bits[i])
            is_zero = 0;
        if (!is_zero)
            putchar (CH_ZERO + v.bits[i]);
    }
    if (is_zero)
        putchar (CH_ZERO);
    int last_digit = HIGH_ACCUR_SIZE;
    for (int i = HIGH_ACCUR_SIZE / 2 - 1; i >= HIGH_ACCUR_SIZE / 2 - len; i--) {
        if (v.bits[i])
            last_digit = i;
    }
    if (HIGH_ACCUR_SIZE / 2 - 1 >= last_digit)
        putchar ('.');
    for (int i = HIGH_ACCUR_SIZE / 2 - 1; i >= last_digit; i--) {
        putchar (CH_ZERO +v. bits[i]);
    }
}

void print_double (struct double_n v, int len) {
    if (is_inf (v) ) {
        if (get_sign (v) == -1)
            putchar ('-');
        printf ("inf\n");
        return;
    }
    if (is_nan (v) ) {
        printf ("nan\n");
        return;
    }
    int exp = get_exp (v) - ( (1LL << (NUM_OF_EXP - 1) ) - 1);
    if (is_denor (v) )
        exp++;
    struct high_accur result;
    struct high_accur base;
    init_accur (&base);
    base.bits[HIGH_ACCUR_SIZE / 2] = 1;
    for (int i = 1; i <= exp; i++) {
        base = mult_2 (base);
    }
    for (int i = -1; i >= exp; i--) {
        base = div_2 (base);
    }
    result = base;
    if (is_denor (v) ) {
        init_accur (&result);
    }
    unsigned long long frac = get_frac (v);
    for (int i = NUM_OF_FRAC - 1; i >= 0; i--)
    {
        base = div_2 (base);
        if (frac & (1LL << i) ) {
            result = h_add (base, result);
        }
    }
    print_accur (result, len);
    printf("\n");
}

void calculator(int len)
{
    init_const_dbl();
    int post_expression[MAX_OPER_LEN];
    struct double_n double_list[MAX_OPER_LEN];
    get_post_expression (post_expression, double_list);
    struct double_n result = calc_post_expression (post_expression, double_list);
    print_double (result, len);
}

int main()
{
    calculator(309);
    return 0;
}
