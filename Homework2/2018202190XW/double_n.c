#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define CASES 10000
#define NUM_OF_BYTES 8
#define NUM_OF_EX_BYTES 8
#define BIT_OF_LONG 64
#define EXP_MIN 2
#define EXP_MAX 12
#define FRAC_MAX 64
#define FRAC_MIN 13
#define EX_MAX 128
#define BIAS 1023
#define EXP_MAX_VALUE 2047
#define BYTE_MAX_VALUE 255
#define NEG 1
#define POS 0
#define EX_LEN (EX_MAX - FRAC_MIN + 1)
#define FRAC_LEN (FRAC_MAX - FRAC_MIN + 1)
struct ex_bytes {
	unsigned char bytes[NUM_OF_EX_BYTES];
}empty_ex;
struct double_n {
	unsigned char bytes[NUM_OF_BYTES];
}NaN_double_n[2], Inf_double_n[2], Zero_double_n[2];
struct int_128 {
	unsigned long long dig[2];
};

//show_bytes
typedef unsigned char *byte_pointer;
void show_bytes(byte_pointer start, size_t len) {
	size_t i;
	for (i = 0; i < len; i++) {
		printf(" %.2x", start[i]);
	}
	printf("\n");
}
void show_double(double x) {
	show_bytes((byte_pointer) &x, sizeof(double));
}
void show_double_n(struct double_n x) {
	show_bytes((byte_pointer) &x, sizeof(struct double_n));
}
void show_ex_bytes(struct ex_bytes x) {
	show_bytes((byte_pointer) &x, sizeof(struct ex_bytes));
}
void prt_bit(struct double_n*, struct ex_bytes*);
void prt_bit_128(struct int_128);

//int_128
void prt_bit_128(struct int_128);
int bigger_128(struct int_128, struct int_128);
int equal_128(struct int_128, struct int_128);
struct int_128 sub_128(struct int_128, struct int_128);
struct int_128 add_128(struct int_128, struct int_128);
void right_shift_128(struct int_128*, int cnt);
void left_shift_128(struct int_128*, int cnt);
struct int_128 long_to_128(unsigned long long);
unsigned long long _128_to_long(struct int_128);
int get_kth_128(struct int_128, int);
struct int_128 pow2_128(int);

//basic operator
int get_kth_digit_double_n(struct double_n*, struct ex_bytes*, int);
void set_kth_digit_double_n(struct double_n*, struct ex_bytes*, int, int);
struct int_128 cut_digit_double_n(struct double_n*, struct ex_bytes*, int, int);
void set_digit_double_n(struct double_n*, struct ex_bytes*, int, int, struct int_128);
void right_shift_double_n(struct double_n*, struct ex_bytes*, int, int, int, int);
void left_shift_double_n(struct double_n*, struct ex_bytes*, int, int, int);
int is_NaN_double_n(struct double_n);
int is_Inf_double_n(struct double_n);
int is_Zero_double_n(struct double_n);
int is_NaN_double(double);
int is_Inf_double(double);
struct double_n rev_double_n(struct double_n);
struct double_n rev_double(double);
int judge(struct double_n, struct double_n);

//swap
void swap_double_n(struct double_n*, struct double_n*);
void swap_long(long long*, long long*);
void swap_int(int*, int*);
void swap_ex_bytes(struct ex_bytes*, struct ex_bytes*);
void swap_128(struct int_128*, struct int_128 *b);

//other operator
long long Max(long long, long long);
long long Min(long long, long long);
long long generate_rand(long long, long long);
struct double_n generate_rand_double_n();

//main oerator
struct double_n shit(int, int, struct int_128);
struct double_n add_n(struct double_n, struct double_n);
struct double_n sub_n(struct double_n, struct double_n);
struct double_n mul_n(struct double_n, struct double_n);
struct double_n div_n(struct double_n, struct double_n);

//debug operator
void init();
void check_calc_double(double, double);
void check_calc_double_n(struct double_n, struct double_n);
void extreme_test();
void rand_test();

//test operator
void generate_input();
void begin_test();
void c_double();
void my_double_n();
void extra_test();

int main() {
	srand(time(0));
//	srand(20000511);
	init();
//	extreme_test();
	rand_test();
	generate_input();
	begin_test();
	extra_test();
	return 0;
}

//int128
void prt_bit(struct double_n *a, struct ex_bytes *b) {
	for (int i = 1; i <= 128; i++) {
		printf("%d", get_kth_digit_double_n(a, b, i));
	}
	printf("\n");
}
void prt_bit_128(struct int_128 a) {
	int aa[130];
	for (int i = 1; i <= 64; i++) {
		aa[i] = a.dig[1] & 1;
		a.dig[1] >>= 1;
	}
	for (int i = 65; i <= 128; i++) {
		aa[i] = a.dig[0] & 1;
		a.dig[0] >>= 1;
	}
	for (int i = 128; i >= 1; i--) printf("%d", aa[i]);
	printf("\n");
}
int bigger_128(struct int_128 a, struct int_128 b) {
	if (a.dig[0] != b.dig[0]) return a.dig[0] > b.dig[0];
	return a.dig[1] > b.dig[1];
}
int equal_128(struct int_128 a, struct int_128 b) {
	return a.dig[1] == b.dig[1] && a.dig[0] == b.dig[0];
}
struct int_128 add_128(struct int_128 a, struct int_128 b) {
	struct int_128 ret;
	ret.dig[0] = a.dig[0] + b.dig[0];
	ret.dig[1] = a.dig[1] + b.dig[1];
	if (ret.dig[1] < a.dig[1]) ret.dig[0]++;
	return ret;
}
struct int_128 sub_128(struct int_128 a, struct int_128 b) {
	struct int_128 ret;
	ret.dig[0] = a.dig[0] - b.dig[0];
	ret.dig[1] = a.dig[1] - b.dig[1];
	if (ret.dig[1] > a.dig[1]) ret.dig[0]--;
	return ret;
}
void right_shift_128(struct int_128 *a, int cnt) {
	if (cnt >= BIT_OF_LONG) {
		(*a).dig[1] = ((*a).dig[0] >> (cnt - BIT_OF_LONG));
		(*a).dig[0] = 0;
		return;
	}
	(*a).dig[1] = ((*a).dig[1] >> cnt) | ((*a).dig[0] << (BIT_OF_LONG - cnt));
	(*a).dig[0] >>= cnt;
}
void left_shift_128(struct int_128 *a, int cnt) {
	if (cnt >= BIT_OF_LONG) {
		(*a).dig[0] = ((*a).dig[1] << (cnt - BIT_OF_LONG));
		(*a).dig[1] = 0;
		return;
	}
	(*a).dig[0] = ((*a).dig[0] << cnt) | ((*a).dig[1] >> (BIT_OF_LONG - cnt));
	(*a).dig[1] <<= cnt;
}
struct int_128 long_to_128(unsigned long long a) {
	struct int_128 ret;
	ret.dig[0] = 0;
	ret.dig[1] = a;
	return ret;
}
unsigned long long _128_to_long(struct int_128 a) {
	return a.dig[1];
}
int get_kth_128(struct int_128 a, int k) {
	right_shift_128(&a, EX_MAX - k);
	return a.dig[1] & 1;
}
struct int_128 pow2_128(int x) {
	struct int_128 ret;
	ret.dig[0] = ret.dig[1] = 0;
	if (x < BIT_OF_LONG) {
		ret.dig[1] = ((unsigned long long) 1) << x;
		return ret;
	}
	x -= BIT_OF_LONG;
	ret.dig[0] = ((unsigned long long) 1) << x;
	return ret;
}

//basic operator
void prt_bit_n(struct double_n *a, struct ex_bytes *b) {
	for (int i = 1; i <= 128; i++) {
		printf("%d", get_kth_digit_double_n(a, b, i));
	}
	printf("\n");
}
int get_kth_digit_double_n(struct double_n *a, struct ex_bytes *b, int k) {
	if (k <= FRAC_MAX) return ((*a).bytes[(k - 1) / 8] >> (7 - (k - 1) % 8)) & 1;
	k -= FRAC_MAX;
	return ((*b).bytes[(k - 1) / 8] >> (7 - (k - 1) % 8)) & 1;
}
void set_kth_digit_double_n(struct double_n *a, struct ex_bytes *b, int k, int value) {
	if (k <= FRAC_MAX) {
		(*a).bytes[(k - 1) / 8] &= (unsigned char) (BYTE_MAX_VALUE - (1 << (7 - (k - 1) % 8)));
		(*a).bytes[(k - 1) / 8] ^= (unsigned char) (value << (7 - (k - 1) % 8));
		return;
	}
	k -= FRAC_MAX;
	(*b).bytes[(k - 1) / 8] &= (unsigned char) (BYTE_MAX_VALUE - (1 << (7 - (k - 1) % 8)));
	(*b).bytes[(k - 1) / 8] ^= (unsigned char) (value << (7 - (k - 1) % 8));
}
struct int_128 cut_digit_double_n(struct double_n *a, struct ex_bytes *b, int l, int r) {
	struct int_128 ret;
	ret.dig[0] = ret.dig[1] = 0;
	for (int i = l; i <= r; i++) {
		left_shift_128(&ret, 1);
		ret.dig[1] |= get_kth_digit_double_n(a, b, i);
	}
	return ret;
}
void set_digit_double_n(struct double_n *a, struct ex_bytes *b, int l, int r, struct int_128 value) {
	for (int i = r; i >= l; i--) {
		set_kth_digit_double_n(a, b, i, value.dig[1] & 1);
		right_shift_128(&value, 1);
	}
}
void right_shift_double_n(struct double_n *a, struct ex_bytes *b, int l, int r, int cnt, int flag) {
	for (int i = r; i >= l; i--) {
		if (i - cnt >= l) {
			int t = get_kth_digit_double_n(a, b, i - cnt);
			set_kth_digit_double_n(a, b, i, t);
		}	
		else if (i - cnt == l - 1) set_kth_digit_double_n(a, b, i, flag);
		else set_kth_digit_double_n(a, b, i, 0);
	}
}
void left_shift_double_n(struct double_n *a, struct ex_bytes *b, int l, int r, int cnt) {
	for (int i = l; i <= r; i++) {
		if (i + cnt <= r) {
			set_kth_digit_double_n(a, b, i, get_kth_digit_double_n(a, b, i + cnt));
		}
		else set_kth_digit_double_n(a, b, i, 0);
	}
}
int is_NaN_double_n(struct double_n a) {
	if (_128_to_long(cut_digit_double_n(&a, &empty_ex, EXP_MIN, EXP_MAX)) == EXP_MAX_VALUE && 
		_128_to_long(cut_digit_double_n(&a, &empty_ex, FRAC_MIN, FRAC_MAX)) != 0) {
		if (get_kth_digit_double_n(&a, &empty_ex, 1) == 0) return 1;
		return -1;
	}
	return 0;
}
int is_Inf_double_n(struct double_n a) {
	if (_128_to_long(cut_digit_double_n(&a, &empty_ex, EXP_MIN, EXP_MAX)) == EXP_MAX_VALUE && 
		_128_to_long(cut_digit_double_n(&a, &empty_ex, FRAC_MIN, FRAC_MAX)) == 0) {
		if (get_kth_digit_double_n(&a, &empty_ex, 1) == 0) return 1;
		return -1;
	}
	return 0;
}
int is_Zero_double_n(struct double_n a) {
	if (equal_128(cut_digit_double_n(&a, &empty_ex, EXP_MIN, FRAC_MAX), long_to_128(0))) {
		if (get_kth_digit_double_n(&a, &empty_ex, 1) == 0) return 1;
		return -1;
	}
	return 0;
}
int is_NaN_double(double a) {
	return is_NaN_double_n(rev_double(a));
}
int is_Inf_double(double a) {
	return is_Inf_double_n(rev_double(a));
}
struct double_n rev_double_n(struct double_n a) {
	struct double_n ret;
	for (int i = 0; i < NUM_OF_BYTES; i++) {
		ret.bytes[i] = 0;
	}
	for (int i = 1; i <= FRAC_MAX; i++) {
		set_kth_digit_double_n(&ret, &empty_ex, (7 - (i - 1) / 8) * 8 + (i - 1) % 8 + 1, get_kth_digit_double_n(&a, &empty_ex, i));
	}
	return ret;
}
struct double_n rev_double(double a) {
	struct double_n ret, a_n = *(struct double_n*) &a;
	for (int i = 0; i < NUM_OF_BYTES; i++) {
		ret.bytes[i] = 0;
	}
	for (int i = 1; i <= FRAC_MAX; i++) {
		set_kth_digit_double_n(&ret, &empty_ex, i, get_kth_digit_double_n(&a_n, &empty_ex, (7 - (i - 1) / 8) * 8 + (i - 1) % 8 + 1));
	}
	return ret;
}
int judge(struct double_n a, struct double_n b) {
	if (is_Zero_double_n(a)) return is_Zero_double_n(a) == is_Zero_double_n(b);
	int flag = 1;
	for (int i = 0; i < NUM_OF_BYTES; i++) if (a.bytes[i] != b.bytes[i]) {
		flag = 0;
		break;
	}
	return flag || is_Inf_double_n(a) && (is_Inf_double_n(a) == is_Inf_double_n(b)) || is_NaN_double_n(a) && (is_NaN_double_n(a) == is_NaN_double_n(b));
}

//swap oeprator
void swap_double_n(struct double_n *a, struct double_n *b) {
	struct double_n tmp = *a;
	*a = *b;
	*b = tmp;
}
void swap_long(long long *a, long long *b) {
	long tmp = *a;
	*a = *b;
	*b = tmp;
}
void swap_int(int *a, int *b) {
	int tmp = *a;
	*a = *b;
	*b = tmp;
}
void swap_ex_bytes(struct ex_bytes *a, struct ex_bytes *b) {
	struct ex_bytes tmp = *a;
	*a = *b;
	*b = tmp;
}
void swap_128(struct int_128 *a, struct int_128 *b) {
	struct int_128 tmp = *a;
	*a = *b;
	*b = tmp;
}

//other operator
long long Max(long long x, long long y) {
	if (x > y) return x;
	return y;
}
long long Min(long long x, long long y) {
	if (x < y) return x;
	return y;
}
long long generate_rand(long long l, long long r) {
	return l + rand() % (r - l + 1);
}
struct double_n generate_rand_double_n() {
	struct double_n ret;
	for (int j = 0; j < NUM_OF_BYTES; j++) {
		ret.bytes[j] = rand();
	}
	int type = rand() % 10;
	if (type < 2) {//+-denormalized_min
		set_digit_double_n(&ret, &empty_ex, EXP_MIN, FRAC_MAX, long_to_128(0));
		if (type & 1) set_kth_digit_double_n(&ret, &empty_ex, 1, 0);
		else set_kth_digit_double_n(&ret, &empty_ex, 1, 1);
		set_kth_digit_double_n(&ret, &empty_ex, FRAC_MAX, 1);
	}
	else if (type < 4) {//+-normalized_max
		set_digit_double_n(&ret, &empty_ex, EXP_MIN, EXP_MAX, long_to_128(EXP_MAX_VALUE - 1));
		set_digit_double_n(&ret, &empty_ex, FRAC_MIN, FRAC_MAX, long_to_128(0ULL - 1ULL));
		if (type & 1) set_kth_digit_double_n(&ret, &empty_ex, 1, 1);
		else set_kth_digit_double_n(&ret, &empty_ex, 1, 0);
	}
	else if (type == 5) {
		return NaN_double_n[rand() % 2];
	}
	else if (type == 6) {
		return Inf_double_n[rand() % 2];
	}
	else if (type == 7) {
		return Zero_double_n[rand() % 2];
	}
	return ret;
}
//main operator
struct double_n shit(int sign, int E, struct int_128 frac) {
	int first_one_pos = 0;
	int flag = 0;
	for (int i = FRAC_MIN - 3; i <= EX_MAX; i++) {
		if (get_kth_128(frac, i)) {
			first_one_pos = i;
			break;
		}
	}
	if (!first_one_pos) return Zero_double_n[sign];
	int left_shift_num = first_one_pos - FRAC_MIN + 1;
	if (E - left_shift_num + BIAS > EXP_MAX_VALUE) return Inf_double_n[sign];
	if (E - left_shift_num + BIAS < 1) {
		left_shift_num = E + BIAS - 1;
		E = -BIAS;
		flag = 1;
	}
	else E -= left_shift_num;
	if (left_shift_num > 0) {
		left_shift_128(&frac, left_shift_num);
	}
	else if (left_shift_num < 0) {
		right_shift_128(&frac, -left_shift_num);
	}
	if (E + BIAS >= EXP_MAX_VALUE) return Inf_double_n[sign];
	int ex_bit = EX_MAX - FRAC_MAX; //64
	if (frac.dig[1] == (((unsigned long long) 1) << (ex_bit - 1))) {
		frac.dig[1] = 0;
		if (frac.dig[0] & 1) {
			frac.dig[0]++;
			return shit(sign, E + flag, frac);
		}
	}
	else if (frac.dig[1] > (((unsigned long long) 1) << (ex_bit - 1))) {
		frac.dig[1] = 0;
		frac.dig[0]++;
		return shit(sign, E + flag, frac);
	}
	else {
		frac.dig[1] = 0;
	}
	struct double_n ret;
	set_kth_digit_double_n(&ret, &empty_ex, 1, sign);
	set_digit_double_n(&ret, &empty_ex, EXP_MIN, EXP_MAX, long_to_128(E + BIAS));
	set_digit_double_n(&ret, &empty_ex, FRAC_MIN, EX_MAX, frac);
	return ret;
}
//----------------------------------------
struct double_n add_n(struct double_n a, struct double_n b) {
	if (is_NaN_double_n(a)) return a;
	if (is_NaN_double_n(b)) return b;
	if (is_Inf_double_n(a) && !is_Inf_double_n(b)) return a;
	if (is_Inf_double_n(b) && !is_Inf_double_n(a)) return b;
	if (is_Inf_double_n(a) && is_Inf_double_n(b)) {
		if (get_kth_digit_double_n(&a, &empty_ex, 1) ^ get_kth_digit_double_n(&b, &empty_ex, 1)) return NaN_double_n[NEG];
		return a;
	}
	struct double_n ret;
	struct ex_bytes a_ex, b_ex, ret_ex;
	int a_E, b_E, a_has_one = 1, b_has_one = 1;
	struct int_128 a_frac, b_frac, ret_frac;
	for (int i = 0; i < NUM_OF_BYTES; i++) ret.bytes[i] = 0;
	for (int i = 0; i < NUM_OF_EX_BYTES; i++) a_ex.bytes[i] = b_ex.bytes[i] = ret_ex.bytes[i] = 0;
	a_E = _128_to_long(cut_digit_double_n(&a, &a_ex, EXP_MIN, EXP_MAX)) - BIAS;
	if (a_E == -BIAS) {
		a_E++;
		a_has_one = 0;
	}
	b_E = _128_to_long(cut_digit_double_n(&b, &b_ex, EXP_MIN, EXP_MAX)) - BIAS;
	if (b_E == -BIAS) {
		b_E++;
		b_has_one = 0;
	}
	int dlt_exp = a_E - b_E;
	if (dlt_exp < 0) {
		swap_double_n(&a, &b);
		swap_int(&a_E, &b_E);
		swap_int(&a_has_one, &b_has_one);
		dlt_exp = -dlt_exp;
	}
	int ret_E = a_E;
	right_shift_double_n(&b, &b_ex, FRAC_MIN, EX_MAX, dlt_exp, b_has_one);
	if (dlt_exp) b_has_one = 0;
	struct int_128 a_one = long_to_128((unsigned long long) a_has_one);
	struct int_128 b_one = long_to_128((unsigned long long) b_has_one);
	left_shift_128(&a_one, EX_LEN);
	left_shift_128(&b_one, EX_LEN);
	a_frac = add_128(cut_digit_double_n(&a, &a_ex, FRAC_MIN, EX_MAX), a_one);
	b_frac = add_128(cut_digit_double_n(&b, &b_ex, FRAC_MIN, EX_MAX), b_one);
	if (bigger_128(b_frac, a_frac) || equal_128(a_frac, b_frac) && get_kth_digit_double_n(&b, &b_ex, 1) == 0) {
		swap_128(&a_frac, &b_frac);
		swap_double_n(&a, &b);
	}
	int sign_dif = get_kth_digit_double_n(&a, &empty_ex, 1) ^ get_kth_digit_double_n(&b, &empty_ex, 1);
	if (sign_dif) ret_frac = sub_128(a_frac, b_frac);
	else ret_frac = add_128(a_frac, b_frac);
	return shit(get_kth_digit_double_n(&a, &empty_ex, 1), ret_E, ret_frac);
}
struct double_n sub_n(struct double_n a, struct double_n b) {
	if (is_NaN_double_n(a)) return a;
	if (is_NaN_double_n(b)) return b;
	set_kth_digit_double_n(&b, &empty_ex, 1, get_kth_digit_double_n(&b, &empty_ex, 1) ^ 1);
	return add_n(a, b);
}
struct double_n mul_n(struct double_n a, struct double_n b) {
	if (is_NaN_double_n(a)) return a;
	if (is_NaN_double_n(b)) return b;
	int sign = get_kth_digit_double_n(&a, &empty_ex, 1) ^ get_kth_digit_double_n(&b, &empty_ex, 1);
	if (is_Inf_double_n(a) && is_Zero_double_n(b)) return NaN_double_n[NEG];
	if (is_Inf_double_n(b) && is_Zero_double_n(a)) return NaN_double_n[NEG];
	if (is_Inf_double_n(a) && !is_Inf_double_n(b) || is_Inf_double_n(b) && !is_Inf_double_n(a)) {
		return Inf_double_n[sign];
	}
	if (is_Inf_double_n(a) && is_Inf_double_n(b)) {
		if (sign) return Inf_double_n[NEG];
		return Inf_double_n[POS];
	}
	struct double_n ret;
	struct ex_bytes a_ex, b_ex, ret_ex;
	int a_E, b_E, a_has_one = 1, b_has_one = 1;
	struct int_128 ret_frac;
	ret_frac.dig[0] = ret_frac.dig[1] = 0;
	for (int i = 0; i < NUM_OF_BYTES; i++) ret.bytes[i] = 0;
	for (int i = 0; i < NUM_OF_EX_BYTES; i++) a_ex.bytes[i] = b_ex.bytes[i] = ret_ex.bytes[i] = 0;
	a_E = _128_to_long(cut_digit_double_n(&a, &a_ex, EXP_MIN, EXP_MAX)) - BIAS;
	if (a_E == -BIAS) {
		a_E++;
		a_has_one = 0;
	}
	b_E = _128_to_long(cut_digit_double_n(&b, &b_ex, EXP_MIN, EXP_MAX)) - BIAS;
	if (b_E == -BIAS) {
		b_E++;
		b_has_one = 0;
	}
	int ret_E = a_E + b_E;
	for (int i = FRAC_MIN - 1; i <= FRAC_MAX; i++) {
		int a_num;
		if (i == FRAC_MIN - 1) a_num = a_has_one;
		else a_num = get_kth_digit_double_n(&a, &a_ex, i); 
		for (int j = FRAC_MIN - 1; j <= FRAC_MAX; j++) {
			int b_num;
			if (j == FRAC_MIN - 1) b_num = b_has_one;
			else b_num = get_kth_digit_double_n(&b, &b_ex, j);
			if (a_num && b_num) ret_frac = add_128(ret_frac, pow2_128(EX_MAX - (i + j - FRAC_MIN + 1)));
		}
	}
	return shit(sign, ret_E, ret_frac);
}
struct double_n div_n(struct double_n a, struct double_n b) {
	int is_NaN_a = is_NaN_double_n(a), is_NaN_b = is_NaN_double_n(b);
	if (is_NaN_a) return a;
	if (is_NaN_b) return b;
	int is_Inf_a = is_Inf_double_n(a), is_Inf_b = is_Inf_double_n(b);
	if (is_Inf_a && is_Inf_b) return NaN_double_n[NEG];
	int sign = get_kth_digit_double_n(&a, &empty_ex, 1) ^ get_kth_digit_double_n(&b, &empty_ex, 1);
	if (is_Inf_a) return Inf_double_n[sign];
	if (is_Inf_b) return Zero_double_n[sign];
	int is_Zero_a = is_Zero_double_n(a), is_Zero_b = is_Zero_double_n(b);
	if (is_Zero_a && is_Zero_b) return NaN_double_n[NEG];
	if (is_Zero_b) return Inf_double_n[sign];
	if (is_Zero_a) return Zero_double_n[sign];
	struct double_n ret;
	struct ex_bytes a_ex, b_ex, ret_ex;
	int a_E, b_E, ret_E, a_has_one = 1, b_has_one = 1;
	struct int_128 a_frac, b_frac, ret_frac;
	ret_frac.dig[0] = ret_frac.dig[1] = 0;
	for (int i = 0; i < NUM_OF_BYTES; i++) ret.bytes[i] = 0;
	for (int i = 0; i < NUM_OF_EX_BYTES; i++) a_ex.bytes[i] = b_ex.bytes[i] = ret_ex.bytes[i] = 0;
	a_E = _128_to_long(cut_digit_double_n(&a, &a_ex, EXP_MIN, EXP_MAX)) - BIAS;
	if (a_E == -BIAS) {
		a_E++;
		a_has_one = 0;
	}
	b_E = _128_to_long(cut_digit_double_n(&b, &b_ex, EXP_MIN, EXP_MAX)) - BIAS;
	if (b_E == -BIAS) {
		b_E++;
		b_has_one = 0;
	}
	int first_one_pos = 0;
	if (b_has_one) first_one_pos = FRAC_MIN - 1;
	else {
		for (int i = FRAC_MIN; i <= FRAC_MAX; i++) {
			if (get_kth_digit_double_n(&b, &b_ex, i) == 1) {
				first_one_pos = i;
				break;
			}
		}
	}
	int left_shift_num = first_one_pos - FRAC_MIN + 1;
	if (left_shift_num > 0) left_shift_double_n(&b, &b_ex, FRAC_MIN, FRAC_MAX, left_shift_num);
	b_has_one = 1;
	b_E -= left_shift_num;
	if (a_has_one) first_one_pos = FRAC_MIN - 1;
	else {
		for (int i = FRAC_MIN; i <= FRAC_MAX; i++) {
			if (get_kth_digit_double_n(&a, &a_ex, i) == 1) {
				first_one_pos = i;
				break;
			}
		}
	}
	left_shift_num = first_one_pos - FRAC_MIN + 1;
	if (left_shift_num > 0) left_shift_double_n(&a, &a_ex, FRAC_MIN, FRAC_MAX, left_shift_num);
	a_has_one = 1;
	a_E -= left_shift_num;
	ret_E = a_E - b_E;
	struct int_128 a_one = long_to_128((unsigned long long) a_has_one);
	struct int_128 b_one = long_to_128((unsigned long long) b_has_one);
	left_shift_128(&a_one, EX_LEN);
	left_shift_128(&b_one, EX_LEN);
	a_frac = add_128(cut_digit_double_n(&a, &a_ex, FRAC_MIN, EX_MAX), a_one);
	b_frac = add_128(cut_digit_double_n(&b, &b_ex, FRAC_MIN, EX_MAX), b_one);
	for (int i = FRAC_MIN - 1; i <= EX_MAX; i++) {
		if (!bigger_128(b_frac, a_frac)) {
			ret_frac = add_128(ret_frac, pow2_128(EX_MAX - i));
			a_frac = sub_128(a_frac, b_frac);
		}
		left_shift_128(&a_frac, 1);
	}
	if ((a_frac.dig[0] != 0 || b_frac.dig[1] != 0)) ret_frac = add_128(ret_frac, long_to_128(1));
	return shit(sign, ret_E, ret_frac);
}
void init() {
	for (int i = 0; i < NUM_OF_EX_BYTES; i++) empty_ex.bytes[i] = 0;
	set_digit_double_n(&NaN_double_n[POS], &empty_ex, EXP_MIN, EXP_MAX, long_to_128((long) EXP_MAX_VALUE));
	set_digit_double_n(&NaN_double_n[NEG], &empty_ex, EXP_MIN, EXP_MAX, long_to_128((long) EXP_MAX_VALUE));
	set_kth_digit_double_n(&NaN_double_n[NEG], &empty_ex, 1, 1);
	set_digit_double_n(&Inf_double_n[POS], &empty_ex, EXP_MIN, EXP_MAX, long_to_128((long) EXP_MAX_VALUE));
	set_digit_double_n(&Inf_double_n[NEG], &empty_ex, EXP_MIN, EXP_MAX, long_to_128((long) EXP_MAX_VALUE));
	set_kth_digit_double_n(&Inf_double_n[NEG], &empty_ex, 1, 1);
	set_kth_digit_double_n(&NaN_double_n[POS], &empty_ex, FRAC_MIN, 1);
	set_kth_digit_double_n(&NaN_double_n[NEG], &empty_ex, FRAC_MIN, 1);
	set_kth_digit_double_n(&Zero_double_n[NEG], &empty_ex, 1, 1);
}
void check_calc_double(double r_a, double r_b) {
	struct double_n a, b, a_add_b, a_sub_b, a_mul_b, a_div_b;
	struct double_n rev_a, rev_b, rev_a_add_b, rev_a_sub_b, rev_a_mul_b, rev_a_div_b;
	double r_a_add_b, r_a_sub_b, r_a_mul_b, r_a_div_b;
	a = rev_double(r_a);
	b = rev_double(r_b);
	rev_a = rev_double_n(a);
	rev_b = rev_double_n(b);
	a_add_b = add_n(a, b);
	r_a_add_b = r_a + r_b;
	rev_a_add_b = rev_double_n(a_add_b);
	a_sub_b = sub_n(a, b);
	r_a_sub_b = r_a - r_b;
	rev_a_sub_b = rev_double_n(a_sub_b);
	a_mul_b = mul_n(a, b);
	r_a_mul_b = r_a * r_b;
	rev_a_mul_b = rev_double_n(a_mul_b);
	a_div_b = div_n(a, b);
	r_a_div_b = r_a / r_b;
	rev_a_div_b = rev_double_n(a_div_b);
	if (!judge(a_add_b, rev_double(r_a_add_b))) {
		printf("%23.15le + %23.15le = %23.15le\n", *(double*) &rev_a, *(double*) &rev_b, *(double*) &rev_a_add_b);
		printf("%23.15le + %23.15le = %23.15le\n", r_a, r_b, r_a_add_b);
		assert(0);
	}
	if (!judge(a_sub_b, rev_double(r_a_sub_b))) {
		printf("%23.15le - %23.15le = %23.15le\n", *(double*) &rev_a, *(double*) &rev_b, *(double*) &rev_a_sub_b);
		printf("%23.15le - %23.15le = %23.15le\n", r_a, r_b, r_a_sub_b);
		assert(0);
	}
	if (!judge(a_mul_b, rev_double(r_a_mul_b))) {
		printf("%23.15le * %23.15le = %23.15le\n", *(double*) &rev_a, *(double*) &rev_b, *(double*) &rev_a_mul_b);
		printf("%23.15le * %23.15le = %23.15le\n", r_a, r_b, r_a_mul_b);
		prt_bit(&a_mul_b, &empty_ex);
		struct double_n fuck = rev_double(r_a_mul_b);
		prt_bit(&fuck, &empty_ex);
		assert(0);
	}
	if (!judge(a_div_b, rev_double(r_a_div_b))) {
		printf("%23.15le / %23.15le = %23.15le\n", *(double*) &rev_a, *(double*) &rev_b, *(double*) &rev_a_div_b);
		printf("%23.15le / %23.15le = %23.15le\n", r_a, r_b, r_a_div_b);
		prt_bit(&a_div_b, &empty_ex);
		struct double_n fuck = rev_double(r_a_div_b);
		prt_bit(&fuck, &empty_ex);
		assert(0);
	}
}
void check_calc_double_n(struct double_n a, struct double_n b) {
	struct double_n rev_a, rev_b;
	rev_a = rev_double_n(a);
	rev_b = rev_double_n(b);
	check_calc_double(*(double*) &rev_a, *(double*) &rev_b);
}
void extreme_test() {
	for (int i = 1; i <= 8; i++) {
		struct double_n a, b, a_add_b, a_sub_b, a_mul_b, a_div_b;
		struct double_n rev_a, rev_b, rev_a_add_b, rev_a_sub_b, rev_a_mul_b, rev_a_div_b;
		double r_a, r_b, r_a_add_b, r_a_sub_b, r_a_mul_b, r_a_div_b;
		if (i == 1) a = NaN_double_n[0];
		if (i == 2) a = NaN_double_n[1];
		if (i == 3) a = Inf_double_n[0];
		if (i == 4) a = Inf_double_n[1];
		if (i == 5) a = Zero_double_n[0];
		if (i == 6) a = Zero_double_n[1];
		if (i == 7) a = rev_double(1);
		if (i == 8) a = rev_double(-1);
		rev_a = rev_double_n(a);
		r_a = *(double*) &rev_a;
		for (int j = 1; j <= 8; j++) {
			if (j == 1) b = NaN_double_n[0];
			if (j == 2) b = NaN_double_n[1];
			if (j == 3) b = Inf_double_n[0];
			if (j == 4) b = Inf_double_n[1];
			if (j == 5) b = Zero_double_n[0];
			if (j == 6) b = Zero_double_n[1];
			if (j == 7) b = rev_double(1);
			if (j == 8) b = rev_double(-1);
			check_calc_double_n(a, b);
		}
	}
}
void rand_test() {
	struct double_n a, b, a_add_b, a_sub_b, a_mul_b, a_div_b;
	struct double_n rev_a, rev_b, rev_a_add_b, rev_a_sub_b, rev_a_mul_b, rev_a_div_b;
	double r_a, r_b, r_a_add_b, r_a_sub_b, r_a_mul_b, r_a_div_b;
	long long test_cnt = 0;
	while (1) {
	//for (int i = 0; i < CASES; i++) {
		test_cnt++;
		if (test_cnt % 100000 == 0) printf("%lld\n", test_cnt);
		a = generate_rand_double_n();
		b = generate_rand_double_n();
		check_calc_double_n(a, b);
	}
}
void generate_input() {
	FILE *output;
	output = fopen("double.in", "w");
	struct double_n a, b;
	for (int i = 0; i < CASES; i++) {
		a = generate_rand_double_n();
		b = generate_rand_double_n();
		struct double_n rev_a, rev_b;
		rev_a = rev_double_n(a);
		rev_b = rev_double_n(b);
		double r_a, r_b;
		r_a = *(double*) &rev_a;
		r_b = *(double*) &rev_b;
		int tmp = rand() % 4;
		char operator;
		if (tmp == 0) operator = '+';
		if (tmp == 1) operator = '-';
		if (tmp == 2) operator = '*';
		if (tmp == 3) operator = '/';
		fprintf(output, "%.50le %c %.50le\n", r_a, operator, r_b);
	}
	fclose(output);
}
void begin_test() {
	c_double();
	my_double_n();
}
void c_double() {
	FILE *input, *output0, *output1;
	input = fopen("double.in", "r");
	output0 = fopen("c_double_0.out", "w");
	output1 = fopen("c_double_1.out", "w");
	char s[10];
	double r_a, r_b, r_ret;
	while (fscanf(input, "%lf %s %lf", &r_a, s, &r_b) != EOF) {
		if (s[0] == '+') {
			r_ret = r_a + r_b;
		}
		if (s[0] == '-') {
			r_ret = r_a - r_b;
		}
		if (s[0] == '*') {
			r_ret = r_a * r_b;
		}
		if (s[0] == '/') {
			r_ret = r_a / r_b;
		}
		fprintf(output0, "%le\n", r_ret);
		fprintf(output1, "%.50le\n", r_ret);
	}
	fclose(input);
	fclose(output0);
	fclose(output1);
}
void my_double_n() {
	FILE *input, *output0, *output1;
	input = fopen("double.in", "r");
	output0 = fopen("my_double_0.out", "w");
	output1 = fopen("my_double_1.out", "w");
	char s[10];
	double r_a, r_b, r_ret;
	while (fscanf(input, "%lf %s %lf", &r_a, s, &r_b) != EOF) {
		struct double_n a, b, ret;
		struct double_n rev_ret;
		a = rev_double(r_a);
		b = rev_double(r_b);
		if (s[0] == '+') {
			ret = add_n(a, b);
		}
		if (s[0] == '-') {
			ret = sub_n(a, b);
		}
		if (s[0] == '*') {
			ret = mul_n(a, b);
		}
		if (s[0] == '/') {
			ret = div_n(a, b);
		}
		rev_ret = rev_double_n(ret);
		fprintf(output0, "%le\n", *(double*) &rev_ret);
		fprintf(output1, "%.50le\n", *(double*) &rev_ret);
	}
	fclose(input);
	fclose(output0);
	fclose(output1);
}
void extra_test() {
	double r_a, r_b, r_ret;
	struct double_n a, b, ret, rev_ret;
	char s[10];
	printf("Please input the expression(input \"0 v 0\" to exit):\n");
	scanf("%lf%s%lf", &r_a, s, &r_b);
	a = rev_double(r_a);
	b = rev_double(r_b);
	if (s[0] == '+') {
		ret = add_n(a, b);
		r_ret = r_a + r_b;
	}
	if (s[0] == '-') {
		ret = sub_n(a, b);
		r_ret = r_a - r_b;
	}
	if (s[0] == '*') {
		ret = mul_n(a, b);
		r_ret = r_a * r_b;
	}
	if (s[0] == '/') {
		ret = div_n(a, b);
		r_ret = r_a / r_b;
	}
	if (s[0] == 'v') return;
	rev_ret = rev_double_n(ret);
	printf("c_double0  : %le\n", r_ret);
	printf("my_double0 : %le\n", *(double*) &rev_ret);
	printf("c_double1  : %.50le\n", r_ret);
	printf("my_double1 : %.50le\n", *(double*) &rev_ret);
	extra_test();
	return;
}
