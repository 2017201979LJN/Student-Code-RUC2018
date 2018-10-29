#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#define NUM_OF_BYTES 8
#define NUM_OF_EXP 11
#define NUM_OF_FRAC 52
#define NUM_OF_FR 27
#define NUM_EX_SGN 63

#define NUM_OF_EXPRESSION 10000

typedef unsigned short bool;
static bool false = 0;
static bool true = 1;

// has_digits: check bits of a certain number
// @param mode: 1  = a has more than b bits
// 				0  = a has b bits
// 				-1 = a has less than b bits 
bool  has_digits(unsigned long long a, int bits, int mode){
	if(mode == 1) 		return (a >> bits) != 0x0ll;
	else if(mode == -1) 
		if(bits <= 0)  return false;
		else  return a < (0x1ll << (bits - 1));
	else if(mode == 0)
		return !has_digits(a, bits, 1) && !has_digits(a, bits, -1);
	return false;
} 

struct my_double{
	unsigned char bytes[NUM_OF_BYTES];
};


// convert_char_to_num: convert char[] to sgn +  exp + frac
void  convert_char_to_num(char* ch, bool* sgn, int* exp, unsigned long long* frac){
	unsigned long long f = 0ll;
	memcpy(&f, ch, sizeof(double));
	*sgn = (bool)((f >> NUM_EX_SGN) & ((0x1ll << 1) - 1));
	*exp = (unsigned int)((f >> NUM_OF_FRAC) & ((0x1ll << NUM_OF_EXP) - 1));
	*frac = f & ((0x1ll << NUM_OF_FRAC) -1);
	return ;
}

// convert_num_to_char: convert sgn + exp + frac -> ch[]
void convert_num_to_char(char* ch, bool* sgn, int* exp, unsigned long long* frac){
	unsigned long long f = 0ll;
	f = ((unsigned long long)(*sgn) << NUM_EX_SGN) | 
		((unsigned long long)(*exp) << NUM_OF_FRAC) | 
		*frac;
	memcpy(ch, &f, sizeof(double));
	return ;
}

// md_double_to_md: convert system double -> my_double
struct my_double  md_double_to_md(double a){
	struct my_double r;
	memcpy(r.bytes, &a, sizeof(double));
	return r;
}

// md_md_to_double: convert my_double -> system double
double  md_md_to_double(struct my_double a){
	double res = 0.0; 
	memcpy(&res, a.bytes, sizeof(double));
	return res;
}



// md_trap: flow for debug
void  md_trap(int mode){
	#define MD_OVERFLOW 1
	#define MD_UNDERFLOW 2
	//#define __ENABLE_TRAP_OUTPUT
	#ifdef __ENABLE_TRAP_OUTPUT
	if (mode == MD_OVERFLOW)	   printf("Overflow!\n");
	else if(mode == MD_UNDERFLOW)  printf("Underflow!\n");
	#endif
	return ;
}

// md_new: create a new my_double
struct my_double  md_new(bool sgn, unsigned int exp, unsigned long long frac){
	struct my_double p;
	convert_num_to_char(p.bytes, &sgn, &exp, &frac);
	return p;
}

// md_change_sign: change the sign with b  
// a = a * sgn(b)
struct my_double md_change_sign(struct my_double a, bool b){
	bool sgn;
	unsigned int exp;
	unsigned long long frac;
	convert_char_to_num(a.bytes, &sgn, &exp, &frac);
	return md_new(sgn ^ b, exp, frac);
}


struct my_double MD_INF_P, MD_INF_N,
				 MD_NAN_P, MD_NAN_N,
				 MD_ZER_P, MD_ZER_N;


// MD_NUM : 0.0, 1.0, 2.0, ....., 10.0
struct my_double MD_NUM[11];

// md_load_special:  initialize special data
void  md_load_special(void){
	MD_INF_P = md_new(0, 0x7ff, 0x0ll);
    MD_INF_N = md_new(1, 0x7ff, 0x0ll);
    MD_NAN_P = md_new(0, 0x7ff, 0x8000000000000ll);
    MD_NAN_N = md_new(1, 0x7ff, 0x8000000000000ll);
    MD_ZER_P = md_new(0, 0x0, 0x0ll);
    MD_ZER_N = md_new(1, 0x0, 0x0ll);
    
    int i;
	for(i = 0; i < 11; i++)
		MD_NUM[i] = md_double_to_md(1.0 * i);
	return ;
}




// md_equal: compares two numbers 
bool  md_equal(struct my_double a, struct my_double b){
	bool sgn_a, sgn_b;
	unsigned int exp_a, exp_b;
	unsigned long long frac_a, frac_b;
	convert_char_to_num(a.bytes, &sgn_a, &exp_a, &frac_a);
	convert_char_to_num(b.bytes, &sgn_b, &exp_b, &frac_b);
	return sgn_a == sgn_b && exp_a == exp_b && frac_a == frac_b;
}

// md_bigger: check  a > b
bool  md_bigger(struct my_double a,struct my_double b){
	bool 				s_1, s_2;
	unsigned int 		e_1, e_2;
	unsigned long long  f_1, f_2;
	convert_char_to_num(a.bytes, &s_1, &e_1, &f_1);
	convert_char_to_num(b.bytes, &s_2, &e_2, &f_2);	

	// 1 negative    1 positive 	
	if(s_1 != s_2) 
		return s_2 == true;
	
	// both negative/positive
	int sgn = 0, ans = 0;
	
	if(s_1 == 1) sgn = 1;
	
	// exp_1 > exp_2
	if(e_1 > e_2) 
		ans = 1;
	// exp_1 = exp_2     frac_1 > frac_2
	else if(e_1 == e_2 && f_1 > f_2 ) 
		ans = 1;
	
	return sgn^ans;	
}






// md_is_nan: returns true if number is +-NaN
// md_is_inf: returns true if number is +-Infinity
// md_is_zer: returns true if number is +- 0
bool  md_is_nan(struct my_double a, int sgn){
	if(sgn == 1)	return md_equal(a, MD_NAN_P);
	if(sgn == -1)	return md_equal(a, MD_NAN_N);
	return md_equal(a, MD_NAN_P) || md_equal(a, MD_NAN_N);
}
bool  md_is_inf(struct my_double a, int sgn){
	if(sgn == 1)	return md_equal(a, MD_INF_P);
	if(sgn == -1)	return md_equal(a, MD_INF_N);
	return md_equal(a, MD_INF_P) || md_equal(a, MD_INF_N);
}
bool  md_is_zer(struct my_double a, int sgn){
	if(sgn == 1)	return md_equal(a, MD_ZER_P);
	if(sgn == -1)	return md_equal(a, MD_ZER_N);
	return md_equal(a, MD_ZER_P) || md_equal(a, MD_ZER_N);
}


// md_add_raw: add / subtract two double numbers, both numbers must be positive.
// @param mode: 1 for addition, -1 for subtraction
struct my_double  md_add_raw(struct my_double a, struct my_double b, int mode){
	bool 				s_1, s_2;
	unsigned int 		e_1, e_2;
	int 		 		e_3 = 0;
	unsigned long long  f_1, f_2, f_3 = 0ll;
	convert_char_to_num(a.bytes, &s_1, &e_1, &f_1);
	convert_char_to_num(b.bytes, &s_2, &e_2, &f_2);					
						
	// Append the invisible 1 					
	f_1 |= 0x1ll << NUM_OF_FRAC;
	f_2 |= 0x1ll << NUM_OF_FRAC;
	
	// Callibrate digits' precision to a same level
	for (; e_1 < e_2; e_1 += 1, f_1 >>= 1);
	for (; e_2 < e_1; e_2 += 1, f_2 >>= 1);
	
	// Perform addition / subtraction
	bool sgn = false;
	if (mode == 1){
		e_3 = e_1;
		f_3 = f_1 + f_2;
		// Remove the first digits to fit 53 bits
		for (; has_digits(f_3, NUM_OF_FRAC+2, 1); e_3 += 1, f_3 >>= 1); 
		// Rounding
		if (has_digits(f_3, NUM_OF_FRAC+1, 1)){
			if (f_3 & 0x1 == 0x1)
				f_3 += 0x2;
			e_3 += 1;
			f_3 >>= 1;
		}
		f_3 &= (0x1ll << NUM_OF_FRAC) - 1;
		// Overflow
		if (e_3 > (1 << NUM_OF_EXP) - 1){
			md_trap(MD_OVERFLOW);
			return md_change_sign(MD_INF_P, sgn);	
		}	
	}
	else if (mode == -1){
		e_3 = e_1;
		if (f_1 > f_2){
			f_3 = f_1 - f_2;
			sgn = false;
		}
		else {
			f_3 = f_2 - f_1;
			sgn = true;
		}
		// Add digits to fit 53 bits
		for (;has_digits(f_3, NUM_OF_FRAC+1, -1); e_3 -= 1, f_3 <<= 1);
		f_3 &= (0x1ll << NUM_OF_FRAC) - 1;
		// Underflow
		if (e_3 < 0){
			md_trap(MD_UNDERFLOW);
			return md_change_sign(MD_ZER_P, sgn);
		} 
	}
	return md_new(sgn, e_3, f_3);
}

// md_mul_raw: Multiple two double numbers, both numbers must be positive.
struct my_double  md_mul_raw(struct my_double a, struct my_double b){
	// Due to the overflow problem of multiplying two 52 digit long long numbers, we need to split them into two:
    // Let a = 2^27 a_1 + b_1, b = 2^27 a_2 + b_2
    // Therefore (a * b) / 2^54 = (a_1 a_2) + (a_1 b_2 + a_2 b_1) / 2^27 +
    //     (b_1 b_2) / 2^54
    // = (a_1 a_2) + (a_1 b_2 + a_2 b_1) / 2^27

	bool 				s_1, s_2;
	unsigned int 		e_1, e_2;
	int 		 		e_3 = 0;
	unsigned long long  f_1, f_2, f_3 = 0ll;
	convert_char_to_num(a.bytes, &s_1, &e_1, &f_1);
	convert_char_to_num(b.bytes, &s_2, &e_2, &f_2);	
	unsigned long long a_1, b_1, a_2, b_2;
	
	// Append the invisible 1 to the 2 frac
    // And add a trailing 0 for ease of computation
	f_1 = (f_1 | (0x1ll << NUM_OF_FRAC)) << 1;
	f_2 = (f_2 | (0x1ll << NUM_OF_FRAC)) << 1;
	// Split f_1 and f_2
	a_1 = (f_1 >> NUM_OF_FR) & ((0x1ll << NUM_OF_FR) - 1);
	b_1 = f_1 & ((0x1ll << NUM_OF_FR) - 1);
	a_2 = (f_2 >> NUM_OF_FR) & ((0x1ll << NUM_OF_FR) - 1);
	b_2 = f_2 & ((0x1ll << NUM_OF_FR) - 1);
	// Calculate	
	f_3 = (a_1 * a_2) + ((a_1 * b_2 + a_2 * b_1) >> NUM_OF_FR);
	e_3 = (int)(e_1) + (int)(e_2) - ((1 << (NUM_OF_EXP - 1)) - 1);
	
	for(; has_digits(f_3, NUM_OF_FRAC + 1, -1); e_3 -= 1,f_3 <<= 1);
	for(; has_digits(f_3, NUM_OF_FRAC + 1,  1); e_3 += 1,f_3 >>= 1);
	
	if(has_digits(f_3, NUM_OF_FRAC + 1, 1)){
		// Rounding
		if(f_3 & 0x1 == 0x1)
			f_3 += 0x2;
		e_3 += 1;
		f_3 >>= 1;		
	}
	f_3 &= (0x1ll << NUM_OF_FRAC) - 1;
	
	// Handle overflow & underflow
	if (e_3 < 0){
		md_trap(MD_UNDERFLOW);
		return MD_ZER_P;
	}
	if(e_3 > (1 << NUM_OF_EXP) - 1){
		md_trap(MD_OVERFLOW);
		return MD_INF_P;
	}
	return md_new(false, e_3, f_3);
}

// md_div_raw: Division between two double numbers, both numbers positive.
struct my_double  md_div_raw(struct my_double a, struct my_double b){
    // Shift 53 bits one by one and create 53 bits of result
    // Take 11111111 / 11000000 for example:
    // 1. 11111111 > 11000000, subtract.
    // 2. 00111111 / 11000000 -> 1-------.
    // 3. Left shift.
    // 4. 01111110 < 11000000, do not subtract.
    // 5. 01111110 / 11000000 -> 10------.
    // 6. Left shift.
    // 7. 11111100 > 11000000, subtract.
    // 8. 00111100 / 11000000 -> 101-----.
    // 9. ...
    
	bool 				s_1, s_2;
	unsigned int 		e_1, e_2;
	int 		 		e_3 = 0;
	unsigned long long  f_1, f_2, f_3 = 0LL;
	convert_char_to_num(a.bytes, &s_1, &e_1, &f_1);
	convert_char_to_num(b.bytes, &s_2, &e_2, &f_2);	
	
	int i;
	// Append the invisible 1 to two frac
	f_1 |= 0x1LL << NUM_OF_FRAC;
	f_2 |= 0x1LL << NUM_OF_FRAC;
	// Division
	for (i = 0; i < NUM_OF_FRAC + 2; i++){
		int newbit = 0;
		if(f_1 >= f_2){
			f_1 -= f_2;
			newbit = 1;
		} 
		f_1 <<=1;
		f_3 = (f_3 << 1) | newbit;
	}
	// Under the case of d_1 < d_2 initially, whereas d_3 < "1", we must callibrate its digits to make "1" <= d_3 < "2".
    // So we let d_3 be 54 bits to preserve precision in the case above.
    // Now we have 54 digits in d_3.
	e_3 = (int)e_1 - (int)e_2 + ((1 << (NUM_OF_EXP - 1)) - 1);
	for (; has_digits(f_3, NUM_OF_FRAC + 2, -1); e_3 -= 1, f_3 <<= 1);
	f_3 = (f_3 >> 1) & ((0x1ll << NUM_OF_FRAC) - 1); 
	// Handle overflow & underflow
	if (e_3 < 0){
		md_trap(MD_UNDERFLOW);
		return MD_ZER_P;
	}
	if (e_3 > ((1 << NUM_OF_EXP) - 1)){
		md_trap(MD_OVERFLOW);
		return MD_INF_P;
	}
	return md_new(false, e_3, f_3);
}

// md_add: Add two double numbers.
struct my_double  md_add(struct my_double a, struct my_double b){
	
	// NaN + x = NaN
	if(md_is_nan(a, 1) || md_is_nan(b, 0))
		return MD_NAN_P;
	// +Inf + - Inf = NaN
	if((md_is_inf(a, 1)  && md_is_inf(b, -1)) || 
	   (md_is_inf(a, -1) && md_is_inf(b, 1)))
		return MD_NAN_P;
	// +Inf + +Inf = +Inf
	if(md_equal(a, b) && md_is_inf(a, 0))
		return a;
	// +Inf + x = +Inf	
	if(md_is_inf(a, 1) || md_is_inf(b, 1))
		return MD_INF_P;
	if(md_is_inf(a, -1) || md_is_inf(b, -1))
		return MD_INF_N;
	
	bool 				s_1, s_2;
	unsigned int 		e_1, e_2;
	unsigned long long  f_1, f_2;
	convert_char_to_num(a.bytes, &s_1, &e_1, &f_1);
	convert_char_to_num(b.bytes, &s_2, &e_2, &f_2);	
	
	bool sgn = s_1;
	
	struct my_double c;
	if (sgn){
		s_1 ^= 1;
		s_2 ^= 1;
		convert_num_to_char(a.bytes, &s_1, &e_1, &f_1);
		convert_num_to_char(b.bytes, &s_2, &e_2, &f_2);
	}
	if(s_2 == false)
		c = md_add_raw(a, b, 1);
	else 
		c = md_add_raw(a, b, -1);
	
	md_change_sign(c, sgn);
	return c;
}  

// md_sub: Subtract two double numbers.
struct my_double  md_sub(struct my_double a, struct my_double b){
	// NaN - x   || x - NaN   = NaN
	if(md_is_nan(a, 0) || md_is_nan(b, 0))
		return MD_NAN_P;
	// +Inf - -Inf = +Inf
	if(md_equal(a, b) && md_is_inf(a, 0))
		return MD_NAN_P;
	// +Inf - x = +Inf
	if((md_is_inf(a, 1) && md_is_inf(b, -1)) || (md_is_inf(b, 1) && md_is_inf(a,-1)))
		return a;
	if(md_is_inf(a, 1) || md_is_inf(b, -1))
		return MD_INF_P;
	if(md_is_inf(a, -1) || md_is_inf(b, 1))	
		return MD_INF_N;
	
	bool 				s_1, s_2;
	unsigned int 		e_1, e_2;
	unsigned long long  f_1, f_2;
	convert_char_to_num(a.bytes, &s_1, &e_1, &f_1);
	convert_char_to_num(b.bytes, &s_2, &e_2, &f_2);	
	
	bool sgn = s_1;
	
	struct my_double c;
	if (sgn){
		s_1 ^= 1;
		s_2 ^= 1;
		convert_num_to_char(a.bytes, &s_1, &e_1, &f_1);
		convert_num_to_char(b.bytes, &s_2, &e_2, &f_2);
	}
	if(s_2 == false)
		c = md_add_raw(a, b, -1);
	else 
		c = md_add_raw(a, b, 1);
	
	md_change_sign(c, sgn);
	return c;
}

// md_mul: Multiply two double numbers.
struct my_double  md_mul(struct my_double a, struct my_double b){
	bool 				s_1, s_2;
	unsigned int 		e_1, e_2;
	unsigned long long  f_1, f_2;
	convert_char_to_num(a.bytes, &s_1, &e_1, &f_1);
	convert_char_to_num(b.bytes, &s_2, &e_2, &f_2);	

	// NaN * x = NaN
	if (md_is_nan(a, 0) || md_is_nan(b, 0))
		return MD_NAN_P;
	// Inf * 0 = NaN
	if ((md_is_inf(a, 0) && md_is_zer(b, 0)) ||
	    (md_is_inf(b, 0) && md_is_zer(a, 0)))
		return md_change_sign(MD_NAN_P, s_1 ^ s_2);
	// Inf * Inf = Inf
	if (md_is_inf(a, 0) && md_is_inf(b, 0))
		return md_change_sign(MD_INF_P, s_1 ^ s_2);
	// Inf * n = Inf, 0 * n = 0
	if (md_is_inf(a, 0) || md_is_zer(a, 0))
		return md_change_sign(a, s_2);
	if (md_is_inf(b, 0) || md_is_zer(b, 0))	 
		return md_change_sign(b, s_1);
	
	bool sgn = s_1 ^ s_2,
		 s_0 = false;
	convert_num_to_char(a.bytes, &s_0, &e_1, &f_1);
	convert_num_to_char(b.bytes, &s_0, &e_2, &f_2);
	
	return md_change_sign(md_mul_raw(a, b), sgn);
}

// md_div: Division between two double numbers.
struct my_double  md_div(struct my_double a, struct my_double b){
	bool 				s_1, s_2;
	unsigned int 		e_1, e_2;
	unsigned long long  f_1, f_2;
	convert_char_to_num(a.bytes, &s_1, &e_1, &f_1);
	convert_char_to_num(b.bytes, &s_2, &e_2, &f_2);	

	// NaN / x  || x / NaN = NaN
	if (md_is_nan(a, 0) || md_is_nan(b, 0))
		return MD_NAN_P;
	// Inf / Inf = NaN, 0 / 0 = NaN
	if ((md_is_inf(a, 0) && md_is_inf(b,0)) ||
	   (md_is_zer(a, 0) && md_is_zer(b,0)))
	   return md_change_sign(MD_NAN_P, s_1 ^ s_2);
	// Inf / n = Inf, 0 / n = 0
	if (md_is_inf(a, 0) || md_is_zer(a, 0))
		return md_change_sign(a, s_2);
	// n / Inf = 0, n / 0 = Inf
	if (md_is_inf(b, 0)) 
		return md_change_sign(MD_ZER_P, s_1 ^ s_2);
	if (md_is_zer(b, 0))
		return md_change_sign(MD_INF_P, s_1 ^ s_2);
	bool sgn = s_1 ^ s_2,
		 s_0 = false;
	
	convert_num_to_char(a.bytes, &s_0, &e_1, &f_1);
	convert_num_to_char(b.bytes, &s_0, &e_2, &f_2);
	return md_change_sign(md_div_raw(a, b), sgn);
}


// find_point : find the point from ch[left -> right]
int find_point(char* ch, int left, int right){
	int res = right + 1, i;
	for(i = left; i <= right; i++) 
		if(ch[i] == '.'){
			res = i;
			break;
		}
	return res;
}

struct my_double read_md_from_char(char* ch, int left, int right){
	int point = find_point(ch, left, right), i;	
	struct my_double res = MD_ZER_P;
	
	// res = res * 10 + (ch[i]-'0')
	for(i = left; i < point; i++){
		res = md_mul(res, MD_NUM[10]);
		res = md_add(res, MD_NUM[ch[i] - '0']);
		
	}
	
	// ignore the point
	int exp_10 = 0;
	for(i = point + 1; i <= right; i++){
		res = md_mul(res, MD_NUM[10]);
		res = md_add(res, MD_NUM[ch[i] - '0']);
		
		exp_10++;
	}
	
	// divide 10 ^ exp_10
	for(i = 0; i < exp_10; i++)
		res = md_div(res, MD_NUM[10]);
	return res;
}

double convert_char_to_double(char* ch, int left, int right){
	double res = 0.0;
	int i,point = find_point(ch, left, right);
	// add integer part
	for(i = left; i < point; i++)
		res = res * 10.0 + 1.0 * (ch[i] - '0');
	double temp = 1.0;
	for(i = point + 1; i <= right; i++){
		temp *= 0.1;
		res += temp * (ch[i]-'0');
	}
	return res;
}



// flow_node : struct to store workflow
// type : 1 (    2 )
//        3 +    4 -    5 *    6 /
// 		  0 double
struct flow_node{
	int type;
	struct my_double val;
};

struct flow_node work_flow[NUM_OF_EXPRESSION];

char expression[NUM_OF_EXPRESSION];

// pretreatment : convert input(char[]) to workflow
// return : length of workflow
int pretreatment(char* ch){
	
	int length = strlen(ch);
	int top = 0;
	int i;
	
	for(i = 0; i < length; i++){
		
		++top;
		
		if(ch[i] == '(')
			work_flow[top].type = 1;
		else if(ch[i] == ')')
			work_flow[top].type = 2;
		else if(ch[i] == '+')
			work_flow[top].type = 3;
		else if(ch[i] == '-')
			work_flow[top].type = 4;
		else if(ch[i] == '*')
			work_flow[top].type = 5;
		else if(ch[i] == '/')
			work_flow[top].type = 6;
		else {
			
			// begin of a double
			work_flow[top].type = 0;
			
			int begin_of_double = i;
			while((ch[i + 1] <= '9' && ch[i + 1] >= '0') || ch[i + 1] == '.')	
				i++;
				
			work_flow[top].val = read_md_from_char(ch, begin_of_double, i);
		}
	}
	
	
	return top;
}

// find_pair : find () pairs in work_flow  ans store in pair[]
void find_pair(int* pair, int len){
	int stack[NUM_OF_EXPRESSION], top = 0, i;
	for(i = 1; i <= len; i++){
		if(work_flow[i].type == 1) 
			stack[++top] = i;
		if(work_flow[i].type == 2){
			pair[i] = stack[top];
			pair[stack[top]] = i;
			top--;
		}
	}
	return;
}


// me_calculate : calculate work_flow[left ~ right] 

// work_flow :  (  + - * /  ) * ( + - ) + - / ()		# contains ()+-*/ 
// sub_flow  :                *         + - / 			# contains +-*/
// raw_flow  :                          + -				# contains +-

struct my_double me_calculate(int* pair, int left, int right){
	
	if(left > right) 
		return MD_ZER_P;
	
	struct flow_node sub_flow[right - left + 1];
	int sub_top = 0, i;
	// cal ( )
	for(i = left; i <= right; i++){
		if(work_flow[i].type == 1) {
			// begin of ( )
			struct my_double tmp = me_calculate(pair, i + 1, pair[i] - 1);
			sub_flow[++sub_top].val = tmp;
			sub_flow[sub_top].type = 0;
			i = pair[i];
		}
		else 
			sub_flow[++sub_top] = work_flow[i];
	}
	
	
	// cal */
	struct flow_node raw_flow[sub_top];
	int raw_top = 0;
	for(i = 1; i <= sub_top; i++){
		if(sub_flow[i].type == 5) {
			raw_flow[raw_top].val = md_mul(raw_flow[raw_top].val, sub_flow[i + 1].val);
			i++;
		}
		else if(sub_flow[i].type == 6) {
			raw_flow[raw_top].val = md_div(raw_flow[raw_top].val, sub_flow[i + 1].val);
			i++;
		}
		else 
			raw_flow[++raw_top] = sub_flow[i];
	}
	
	
	
	struct my_double ans = MD_ZER_P;
	i = 1;
	
	// in case that the first number is negative
	if(raw_flow[i].type == 0) {
		ans = md_add(ans, raw_flow[i].val);
		i++;
	}
	
	for( ; i <= raw_top; i = i + 2){
		if(raw_flow[i].type == 3) 
			ans = md_add(ans, raw_flow[i+1].val);
		else 
			ans = md_sub(ans, raw_flow[i+1].val);	
	}
	return ans;
}

// md_digit_output :
// @param a: k.xxxxxxxxxxxxx
//   output: a number k 
void md_digit_output(struct my_double a){
	int i;
	for(i = 9; i >= 0; i--)
		if(md_bigger(a, MD_NUM[i])){
			printf("%d",i);
			break;
		}
	return ;
}

// md_output :  output a my_double number
void md_output(struct my_double a, int precision){
	// example : 123.45678
	 
	struct my_double MD_1  = MD_NUM[1],
					 MD_10 = MD_NUM[10],
					 tmp   = a;
	int digit = 1, i;
	// digit: (of integer)   example : digit = 3 
	
	while(md_bigger(tmp, MD_10)){
		tmp = md_div(tmp, MD_10);
		digit++;
	}
	
	// example tmp = 1.2345678  ->  output 1  ->  tmp = 2.345678 
	
	for(i = 1; i <= digit; i++){
		
		md_digit_output(tmp);
		tmp = md_mul(tmp, MD_10);
		while(md_bigger(tmp, MD_10)) tmp = md_sub(tmp, MD_10);
		
	}
	
	if(precision != 0) printf(".");
	
	for(i = 1; i <= precision; i++){
		md_digit_output(tmp);
		tmp = md_mul(tmp, MD_10);
		while(md_bigger(tmp, MD_10)) tmp = md_sub(tmp, MD_10);
		
	}
	return ;
}


int main(int argc, char** argv)
{
	md_load_special();

	struct my_double md_1, md_2, md_3;
	// read an expression 
	char expression[NUM_OF_EXPRESSION];
	scanf("%s", expression);
	
	// convert to work_flow
	int length_of_flow = pretreatment(expression);
	int pair[NUM_OF_EXPRESSION];
	
	find_pair(pair, length_of_flow);
	
	
	md_output( me_calculate(pair, 1, length_of_flow), 40 );

	return 0;
}
