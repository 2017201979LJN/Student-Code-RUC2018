#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define CASES 10000
#define NUM_OF_BYTES 8
#define NUM_OF_EX_BYTES 8
#define BIT_OF_LONG 64
#define BIT_OF_BYTE 8
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
#define BUF_LEN 1500
#define ROUND_CNT 50
#define LENGTH_OF_SCIENTIFIC 60
#define NUM_OF_ELEMENT_PERCASE 50
#define BASE_DEC 10
#define BASE_BIN 2
#define HUNDRED 100
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

//swap
void swap_double_n(struct double_n*, struct double_n*);
void swap_int(int*, int*);
void swap_ex_bytes(struct ex_bytes*, struct ex_bytes*);
void swap_128(struct int_128*, struct int_128 *b);

//main oerator
struct double_n adjust(int, int, struct int_128);
struct double_n add_n(struct double_n, struct double_n);
struct double_n sub_n(struct double_n, struct double_n);
struct double_n mul_n(struct double_n, struct double_n);
struct double_n div_n(struct double_n, struct double_n);

//
void init();
int getline_(FILE*, char*, int);
void work(FILE*);
int a_to_int(char*, int);
struct double_n a_to_double_n(char*, int);
int double_n_to_a(struct double_n, char*, int);
void div2_dec_a(char*, int);

int main() {
	srand(time(0));
	init();
	FILE* input = fopen("data.in", "r");
	work(input);
	fclose(input);
	return 0;
}

//int128
void prt_bit(struct double_n *a, struct ex_bytes *b) {
	for (int i = 1; i <= EX_MAX; i++) {
		printf("%d", get_kth_digit_double_n(a, b, i));
	}
	printf("\n");
}
void prt_bit_128(struct int_128 a) {
	int aa[EX_MAX + 1];
	for (int i = 1; i <= FRAC_MAX; i++) {
		aa[i] = a.dig[1] & 1;
		a.dig[1] >>= 1;
	}
	for (int i = FRAC_MAX + 1; i <= EX_MAX; i++) {
		aa[i] = a.dig[0] & 1;
		a.dig[0] >>= 1;
	}
	for (int i = EX_MAX; i >= 1; i--) printf("%d", aa[i]);
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
	for (int i = 1; i <= EX_MAX; i++) {
		printf("%d", get_kth_digit_double_n(a, b, i));
	}
	printf("\n");
}
int get_kth_digit_double_n(struct double_n *a, struct ex_bytes *b, int k) {
	if (k <= FRAC_MAX) return ((*a).bytes[(k - 1) / BIT_OF_BYTE] >> (BIT_OF_BYTE  - 1 - (k - 1) % BIT_OF_BYTE)) & 1;
	k -= FRAC_MAX;
	return ((*b).bytes[(k - 1) / BIT_OF_BYTE] >> (BIT_OF_BYTE  - 1 - (k - 1) % BIT_OF_BYTE)) & 1;
}
void set_kth_digit_double_n(struct double_n *a, struct ex_bytes *b, int k, int value) {
	if (k <= FRAC_MAX) {
		(*a).bytes[(k - 1) / BIT_OF_BYTE] &= (unsigned char) (BYTE_MAX_VALUE - (1 << (BIT_OF_BYTE  - 1 - (k - 1) % BIT_OF_BYTE)));
		(*a).bytes[(k - 1) / BIT_OF_BYTE] ^= (unsigned char) (value << (BIT_OF_BYTE  - 1 - (k - 1) % BIT_OF_BYTE));
		return;
	}
	k -= FRAC_MAX;
	(*b).bytes[(k - 1) / BIT_OF_BYTE] &= (unsigned char) (BYTE_MAX_VALUE - (1 << (BIT_OF_BYTE  - 1 - (k - 1) % BIT_OF_BYTE)));
	(*b).bytes[(k - 1) / BIT_OF_BYTE] ^= (unsigned char) (value << (BIT_OF_BYTE  - 1 - (k - 1) % BIT_OF_BYTE));
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
		set_kth_digit_double_n(&ret, &empty_ex, (BIT_OF_BYTE  - 1 - (i - 1) / BIT_OF_BYTE) * BIT_OF_BYTE + (i - 1) % BIT_OF_BYTE + 1, get_kth_digit_double_n(&a, &empty_ex, i));
	}
	return ret;
}
struct double_n rev_double(double a) {
	struct double_n ret, a_n = *(struct double_n*) &a;
	for (int i = 0; i < NUM_OF_BYTES; i++) {
		ret.bytes[i] = 0;
	}
	for (int i = 1; i <= FRAC_MAX; i++) {
		set_kth_digit_double_n(&ret, &empty_ex, i, get_kth_digit_double_n(&a_n, &empty_ex, (BIT_OF_BYTE  - 1 - (i - 1) / BIT_OF_BYTE) * BIT_OF_BYTE + (i - 1) % BIT_OF_BYTE + 1));
	}
	return ret;
}

//swap oeprator
void swap_double_n(struct double_n *a, struct double_n *b) {
	struct double_n tmp = *a;
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

//main operator
struct double_n adjust(int sign, int E, struct int_128 frac) {
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
			return adjust(sign, E + flag, frac);
		}
	}
	else if (frac.dig[1] > (((unsigned long long) 1) << (ex_bit - 1))) {
		frac.dig[1] = 0;
		frac.dig[0]++;
		return adjust(sign, E + flag, frac);
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
	return adjust(get_kth_digit_double_n(&a, &empty_ex, 1), ret_E, ret_frac);
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
	return adjust(sign, ret_E, ret_frac);
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
	return adjust(sign, ret_E, ret_frac);
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

int getline_(FILE* input, char s[],int lim){
	char c;
	int len = 0;
	while (fscanf(input, "%c", &c) != EOF && c != '\n' && len < lim - 1) {
        s[len++] = c;
    }
	s[len] = '\0';
	return len;
}
void work(FILE* input) {
	char s[BUF_LEN];
	char c[BUF_LEN];
	struct double_n number_set[NUM_OF_ELEMENT_PERCASE];
	int len, cnt_all, cnt_number;
	while ((len = getline_(input, s, BUF_LEN)) > 0) {
		int wait_for_number = 1;
		int wait_for_operator = 0;
		cnt_all = cnt_number = 0;
		for (int i = 0; i < len; i++) {
			if (!((s[i] >= '0' && s[i] <= '9') || (s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z') || 
				   s[i] == '.' || s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/' || s[i] == '(' || s[i] == ')')) continue;
			if (wait_for_number) {
				if (s[i] == '(') {
					c[++cnt_all] = '(';
					continue;
				}
				if (s[i] == 'n' && s[i + 1] == 'a' && s[i + 2] == 'n') {
					c[++cnt_all] = '0';
					number_set[++cnt_number] = NaN_double_n[POS];
					wait_for_number = 0;
					wait_for_operator = 1;
					i = i + 2;
					continue;
				}
				if (s[i] == '-' && s[i + 1] == 'n' && s[i + 2] == 'a' && s[i + 3] == 'n') {
					c[++cnt_all] = '0';
					number_set[++cnt_number] = NaN_double_n[NEG];
					wait_for_number = 0;
					wait_for_operator = 1;
					i = i + 3;
					continue;
				}
				if (s[i] == 'i' && s[i + 1] == 'n' && s[i + 2] == 'f') {
					c[++cnt_all] = '0';
					number_set[++cnt_number] = Inf_double_n[POS];
					wait_for_number = 0;
					wait_for_operator = 1;
					i = i + 2;
					continue;
				}
				if (s[i] == '-' && s[i + 1] == 'i' && s[i + 2] == 'n' && s[i + 3] == 'f') {
					c[++cnt_all] = '0';
					number_set[++cnt_number] = Inf_double_n[NEG];
					wait_for_number = 0;
					wait_for_operator = 1;
					i = i + 3;
					continue;
				}
				int end_of_number = i;
				for (int j = i + 1; j < len; j++) {
					end_of_number = j;
					if ((s[j] == '+' || s[j] == '-') && (s[j - 1] == 'E' || s[j - 1] == 'e')) continue;
					if (s[j] == 'e' || s[j] == 'E') continue;
					if (s[j] >= '0' && s[j] <= '9') continue;
					if (s[j] == '.') continue;
					end_of_number--;
					break;
				}
				c[++cnt_all] = '0';
				char tmp[BUF_LEN];
				int tmp_len = 0;
				for (int j = 0; j < end_of_number - i + 1; j++) tmp[tmp_len++] = s[i + j];
				number_set[++cnt_number] = a_to_double_n(tmp, end_of_number - i + 1);
				i = end_of_number;
				wait_for_number = 0;
				wait_for_operator = 1;
			}
			else if (wait_for_operator) {
				if (s[i] == ')') {
					c[++cnt_all] = ')';
					continue;
				}
				c[++cnt_all] = s[i];
				wait_for_number = 1;
				wait_for_operator = 0;
			}
		}
		struct double_n stack_num[NUM_OF_ELEMENT_PERCASE];
		int top_num = 0, top_opt = 0;
		char stack_opt[NUM_OF_ELEMENT_PERCASE];
		int now_cnt_number = 0;
		for (int i = 1; i <= cnt_all; i++) {
			if (c[i] == '0') stack_num[++top_num] = number_set[++now_cnt_number];
			else {
				if (c[i] == '(') {
					stack_opt[++top_opt] = c[i];
				}
				else if (c[i] == '*' || c[i] == '/') {
					while (top_opt && (stack_opt[top_opt] == '*' || stack_opt[top_opt] == '/')) {
						struct double_n b = stack_num[top_num--];
						struct double_n a = stack_num[top_num--];
						struct double_n ret;
						if (stack_opt[top_opt] == '+') ret = add_n(a, b);
						else if (stack_opt[top_opt] == '-') ret = sub_n(a, b);
						else if (stack_opt[top_opt] == '*') ret = mul_n(a, b);
						else ret = div_n(a, b);
						top_opt--;
						stack_num[++top_num] = ret;
					}
					stack_opt[++top_opt] = c[i];
				}
				else if (c[i] == '+' || c[i] == '-') {
					while (top_opt && (stack_opt[top_opt] == '*' || stack_opt[top_opt] == '/' || stack_opt[top_opt] == '+' || stack_opt[top_opt] == '-')) {
						struct double_n b = stack_num[top_num--];
						struct double_n a = stack_num[top_num--];
						struct double_n ret;
						if (stack_opt[top_opt] == '+') ret = add_n(a, b);
						else if (stack_opt[top_opt] == '-') ret = sub_n(a, b);
						else if (stack_opt[top_opt] == '*') ret = mul_n(a, b);
						else ret = div_n(a, b);
						top_opt--;
						stack_num[++top_num] = ret;
					}
					stack_opt[++top_opt] = c[i];
				}
				else if (c[i] == ')') {
					while (stack_opt[top_opt] != '(') {
						struct double_n b = stack_num[top_num--];
						struct double_n a = stack_num[top_num--];
						struct double_n ret;
						if (stack_opt[top_opt] == '+') ret = add_n(a, b);
						else if (stack_opt[top_opt] == '-') ret = sub_n(a, b);
						else if (stack_opt[top_opt] == '*') ret = mul_n(a, b);
						else ret = div_n(a, b);
						top_opt--;
						stack_num[++top_num] = ret;
					}
					top_opt--;
				}
			}
		}
		while (top_opt) {
			struct double_n b = stack_num[top_num--];
			struct double_n a = stack_num[top_num--];
			struct double_n ret;
			if (stack_opt[top_opt] == '+') ret = add_n(a, b);
			else if (stack_opt[top_opt] == '-') ret = sub_n(a, b);
			else if (stack_opt[top_opt] == '*') ret = mul_n(a, b);
			else ret = div_n(a, b);
			top_opt--;
			stack_num[++top_num] = ret;
		}
		char tmp[BUF_LEN];
		int len = double_n_to_a(stack_num[1], tmp, ROUND_CNT);
		for (int j = 0; j < len; j++) printf("%c", tmp[j]); printf("\n");
	}
}
int a_to_int(char s[], int len) {
	int sign, ret = 0;
	if (s[0] == '-') sign = -1;
	else sign = 1;
	for (int i = (s[0] == '-' || s[0] == '+'); i < len; i++) {
		ret = ret * BASE_DEC + s[i] - '0';
	}
	return ret * sign;
}
struct double_n a_to_double_n(char s[], int len) {
	int sign = 0;
	char integral[BUF_LEN];
	int integral_cnt = 0;
	char fractional[BUF_LEN];
	int fractional_cnt = 0;
	int is_scientific = 0, demical_pos, frac = 0;
	if (s[0] == '-') {
		sign = 1;
		s = s + 1;
		len--;
	}
	for (int i = 0; i < len; i++) if (s[i] == 'e' || s[i] == 'E') {
		is_scientific = 1;
		frac = a_to_int(s + i + 1, len - i - 1);
		len = i;
		break;
	}
	demical_pos = len;
	for (int i = 0; i < len; i++) if (s[i] == '.') {
		demical_pos = i;
		break;
	}
	for (int i = 0; i < len; i++) if (s[i] != '.') s[i] -= '0';
	if (demical_pos < len) {
		for (int i = demical_pos; i < len - 1; i++) s[i] = s[i + 1];
		len--;
	}
	demical_pos += frac;
	for (int i = len; i < demical_pos; i++) s[i] = 0;
	if (demical_pos > len) len = demical_pos;
	if (demical_pos < 0) {
		for (int i = len - 1; i >= 0; i--) s[i - demical_pos] = s[i];
		for (int i = 0; i < -demical_pos; i++) s[i] = 0;
		len -= demical_pos;
		demical_pos = 0;
	}
	int first_not_zero_pos = 0;
	while (first_not_zero_pos < demical_pos) {
		integral[integral_cnt++] = s[demical_pos - 1] & 1;
		int res = 0;
		int first_not_zero_pos_tmp = first_not_zero_pos + (s[first_not_zero_pos] <= 1);
		for (int i = first_not_zero_pos; i < demical_pos; i++) {
			int res_tmp = s[i] & 1;
			s[i] = (res * BASE_DEC + s[i]) >> 1;
			res = res_tmp;
		}
		first_not_zero_pos = first_not_zero_pos_tmp;
	}
	for (int i = 0; i < BUF_LEN; i++) {
		int res = 0;
		for (int j = len - 1; j >= demical_pos; j--) {
			int tmp = s[j] * 2 + res;
			s[j] = tmp % BASE_DEC;
			res = tmp / BASE_DEC;
		}
		fractional[fractional_cnt++] = res;
	}
	int num_E = 0;
	int found_first_one = 0;
	for (int i = integral_cnt - 1; i >= 0; i--) if (integral[i]) {
		num_E = i;
		found_first_one = 1;
		struct int_128 frac;
		frac.dig[0] = frac.dig[1] = 0;
		int cnt = 0;
		for (int j = i; j >= 0 && cnt < EX_MAX - FRAC_MIN + 2; j--) {
			left_shift_128(&frac, 1);
			frac.dig[1] += integral[j];
			cnt++;
		}
		for (int j = 0; j < fractional_cnt && cnt < EX_MAX - FRAC_MIN + 2; j++) {
			left_shift_128(&frac, 1);
			frac.dig[1] += fractional[j];
			cnt++;
		}
		if (cnt < EX_MAX - FRAC_MIN + 2) left_shift_128(&frac, EX_MAX - FRAC_MIN + 2 - cnt);
		return adjust(sign, num_E, frac);
	}
	if (!found_first_one) {
		for (int i = 0; i < fractional_cnt; i++) if (fractional[i]) {
			num_E = -i - 1;
			found_first_one = 1;
			struct int_128 frac;
			frac.dig[0] = frac.dig[1] = 0;
			int cnt = 0;
			for (int j = i; j < fractional_cnt && cnt < EX_MAX - FRAC_MIN + 2; j++) {
				left_shift_128(&frac, 1);
				frac.dig[1] += fractional[j];
				cnt++;
			}
			if (cnt < EX_MAX - FRAC_MIN + 2) left_shift_128(&frac, EX_MAX - FRAC_MIN + 2 - cnt);
			return adjust(sign, num_E, frac);
		}
	}
	if (!found_first_one) {
		struct int_128 zero_128;
		zero_128.dig[0] = zero_128.dig[1] = 0;
		return adjust(sign, 0, zero_128);
	}
}
int double_n_to_a(struct double_n a, char s[], int round_cnt) {
	int sign = get_kth_digit_double_n(&a, &empty_ex, 1);
	int len = 0;
	if (is_NaN_double_n(a)) {
		if (sign) s[len++] = '-';
		s[len++] = 'N';
		s[len++] = 'A';
		s[len++] = 'N';
		return len;
	}
	if (is_Inf_double_n(a)) {
		if (sign) s[len++] = '-';
		s[len++] = 'I';
		s[len++] = 'N';
		s[len++] = 'F';
		return len;
	}
	int a_E, a_has_one = 1;
	a_E = _128_to_long(cut_digit_double_n(&a, &empty_ex, EXP_MIN, EXP_MAX)) - BIAS;
	if (a_E == -BIAS) {
		a_E++;
		a_has_one = 0;
	}
	char integral[BUF_LEN];
	int integral_cnt = 0;
	char fractional[BUF_LEN];
	int fractional_cnt = 0;
	for (int i = 0; i <= a_E; i++) {
		if (i == 0) integral[integral_cnt++] = a_has_one;
		else integral[integral_cnt++] = get_kth_digit_double_n(&a, &empty_ex, FRAC_MIN + i - 1);
	}
	for (int i = FRAC_MIN + a_E; i <= FRAC_MAX; i++) {
		if (i < FRAC_MIN - 1) fractional[fractional_cnt++] = 0;
		else if (i == FRAC_MIN - 1) fractional[fractional_cnt++] = a_has_one;
		else fractional[fractional_cnt++] = get_kth_digit_double_n(&a, &empty_ex, i);
	}
//	for (int i = 0; i < integral_cnt; i++) printf("%d", integral[i]); printf("."); for (int i = 0; i< fractional_cnt; i++) printf("%d", fractional[i]);printf("\n");
	s[0] = s[1] = 0;
	for (int i = 0; i < integral_cnt; i++) {
		int res = 0;
		for (int j = 0; j < len; j++) {
			int tmp = s[j] * BASE_BIN + res;
			s[j] = tmp % BASE_DEC;
			res = tmp / BASE_DEC;
		}
		if (res) s[len++] = res;
		s[len] = 0;
		s[0] += integral[i];
		for (int j = 0; j < len; j++) {
			s[j + 1] += s[j] / BASE_DEC;
			s[j] %= BASE_DEC;
		}
		if (s[len]) len++;
		s[len] = 0;
	}
	int demical_pos = len;
	len = BUF_LEN - 1;
	char basic_num[BUF_LEN];
	for (int i = 0; i < len; i++) basic_num[i] = 0;
	basic_num[0] = 5;
	for (int i = demical_pos; i < len; i++) s[i] = 0;
	for (int i = 0; i < fractional_cnt; i++) {
		if (fractional[i]) {
			for (int j = len - 1; j >= demical_pos; j--) {
				s[j - 1] += (s[j] + basic_num[j - demical_pos]) / BASE_DEC;
				s[j] = (s[j] + basic_num[j - demical_pos]) % BASE_DEC;
			}
		}
		div2_dec_a(basic_num, BUF_LEN);
	}
	for (int i = 0; i < demical_pos; i++) basic_num[i] = s[i];
	for (int i = 0; i < demical_pos; i++) s[i] = basic_num[demical_pos - i - 1];
	int first_not_zero_pos = -1;
	for (int i = 0; i < len; i++) if (s[i] != 0) {
		first_not_zero_pos = i;
		break;
	}
	char ret[LENGTH_OF_SCIENTIFIC];
	if (first_not_zero_pos == -1) {
		len = 0;
		if (sign) ret[len++] = '-';
		ret[len++] = '0';
		ret[len++] = '.';
		for (int i = 0; i < round_cnt; i++) ret[len++] = '0';
		ret[len++] = 'E';
		ret[len++] = '+';
		ret[len++] = '0';
		ret[len++] = '0';
		for (int i = 0; i < len; i++) s[i] = ret[i];
		return len;
	}
	len = 0;
	a_E = demical_pos - first_not_zero_pos - 1;
	if (sign) ret[len++] = '-';
	ret[len++] = s[first_not_zero_pos] + '0';
	ret[len++] = '.';
	for (int i = 0; i < round_cnt; i++) ret[len++] = s[first_not_zero_pos + i + 1] + '0';
	if (s[first_not_zero_pos + round_cnt + 1] >= 5) {
		ret[len - 1]++;
		for (int i = len - 1; i > len - round_cnt; i--) {
			if (ret[i] > '9') {
				ret[i - 1]++;
				ret[i] -= BASE_DEC;
			}
		}
		if (ret[len - round_cnt] > '9') {
			ret[len - round_cnt - 2]++;
			ret[len - round_cnt] -= BASE_DEC;
		}
		if (ret[len - round_cnt - 2] > '9') {
			for (int i = len - 1; i > len - round_cnt; i--) ret[i] = ret[i - 1];
			ret[len - round_cnt] = '0';
			ret[len - round_cnt - 2] = '1';
		}
	}
	ret[len++] = 'E';
	if (a_E >= 0) ret[len++] = '+';
	else {
		ret[len++] = '-';
		a_E = -a_E;
	}
	if (a_E >= HUNDRED) ret[len++] = a_E / HUNDRED + '0';
	ret[len++] = a_E % HUNDRED / BASE_DEC + '0';
	ret[len++] = a_E % BASE_DEC + '0';
	for (int i = 0; i < len; i++) s[i] = ret[i];
	return len;
}
void div2_dec_a(char s[], int len) {
	int res = 0;
	for(int i = 0; i < len; i++) {
		res = s[i] + res * BASE_DEC;
		s[i] = (res >> 1);
		res = res & 1;
	}
}
