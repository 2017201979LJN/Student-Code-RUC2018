#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
typedef unsigned char uc;
#define ull unsigned long long
#define N 1005
#define Num_of_Bytes 8
#define Value_of_Bytes 256
#define Half_of_Value 128
#define Length_of_Double 64
#define Length_of_Frac 52
#define Length_of_Exp 12
#define Length_of_Bignum 350
#define Power_of_Five 32
#define Power_of_Four 16
#define Binary_of_Lastfour 15
#define Bias 1023
#define Exp_Max 2047
#define Two 2.0
#define Half 0.5
#define Board_of_Decimal 10
#define Need_of_Bit 5
int n;
char s[N];
struct double_n{
	unsigned char bytes[Num_of_Bytes];
}da,db,dc;
void debug (struct double_n *a) {
	for (int i = 0; i < Num_of_Bytes; i++) {
		for (int j = (Num_of_Bytes - 1); j >= 0; j--)
			printf ("%d", (a->bytes[i] >> j) & 1);
		printf (" ");
	}
	puts ("");
}
void clear (struct double_n *a) {
	for (int i = 0; i < Num_of_Bytes; i++)
		a->bytes[i] = 0;
}
void set (struct double_n *a, int by, int p, int v) {
	a->bytes[by] &= (Value_of_Bytes - 1) - (1 << p);
	a->bytes[by] |= v << p;
}
void setinf (struct double_n *a) {
	a->bytes[0] |= (Half_of_Value - 1);
	a->bytes[1] = (Value_of_Bytes - 1) - Binary_of_Lastfour;
	for (int i = 2; i < Num_of_Bytes; i++)
		a->bytes[i] = 0;
}
void setneg (struct double_n *a) {
	a->bytes[0] ^= 128;
}
void Swap (struct double_n *a, struct double_n *b) {
	for (int i = 0; i < Num_of_Bytes; i++) {
		uc tmp = a->bytes[i];
		a->bytes[i] = b->bytes[i];
		b->bytes[i] = tmp;
	}
}
int iszero (struct double_n *a) {
	return (a->bytes[0] & (Half_of_Value - 1)) == 0 && (a->bytes[1] >> 4) == 0;
}
int is_inf (struct double_n *a) {
	return (a->bytes[0] & (Half_of_Value - 1)) == (Half_of_Value - 1) && ((a->bytes[1] >> 4) & Binary_of_Lastfour) == Binary_of_Lastfour;
}
int is_nan (struct double_n *a) {
	if (is_inf (a) == 0)
		return 0;
	if (a->bytes[1] & Binary_of_Lastfour)
		return 1;
	for (int i = 2; i < Num_of_Bytes; i++)
		if (a->bytes[i])
			return 1;
	return 0;
}
int sym (struct double_n *a, struct double_n *b) {
	return (a->bytes[0] >> (Num_of_Bytes - 1)) ^ (b->bytes[0] >> (Num_of_Bytes - 1));
}
int bigger (struct double_n *a, struct double_n *b) {
	if ((a->bytes[0] & (Half_of_Value - 1)) > (b->bytes[0] & (Half_of_Value - 1))) return 1;
	if ((a->bytes[0] & (Half_of_Value - 1)) < (b->bytes[0] & (Half_of_Value - 1))) return 0;
	for (int i = 1; i < Num_of_Bytes; i++) {
		if (a->bytes[i] > b->bytes [i]) return 1;
		if (a->bytes[i] < b->bytes [i]) return 0;
	}
	return 1;
}
int allzero (struct double_n *a, int b) {
	for (int i = b; i < Length_of_Double; i++) {
		int by = i >> 3, p = Num_of_Bytes - (i & (Num_of_Bytes - 1)) - 1;
		if ((a->bytes[by] >> p) & 1)
			return 0;
	}
	return 1;
}
int calexp (struct double_n *a) {
	int mor = 0;
	for (int i = 1; i < Length_of_Exp; i++) {
		int by = i >> 3, p = Num_of_Bytes - (i & (Num_of_Bytes - 1)) - 1;
		mor <<= 1;
		if ((a->bytes[by] >> p) & 1)
			mor++;
	}
	return mor;
}
struct double_n chu (double x) {
	struct double_n a;
	clear (&a);
	if (x < 0) {
		a.bytes[0] = Half_of_Value;
		x = -x;
	}
	int cnt = 0;
	if (x >= 1.0) {
		while (x >= Two) {
			x /= Two;
			cnt++;
			if (cnt + Bias >= Exp_Max)
				break;
		}
		int tmp = cnt + Bias;
		if (tmp >= Exp_Max) {
			a.bytes[0] |= (Half_of_Value - 1);
			a.bytes[1] = (Value_of_Bytes - 1) - Binary_of_Lastfour;
			return a;
		}
		else {
			a.bytes[0] |= tmp >> 4;
			a.bytes[1] = (tmp & Binary_of_Lastfour) << 4;
			x -= 1.0;
			for (int i = Length_of_Exp; i < Length_of_Double; i++) {
				x *= Two;
				if (x >= 1.0) {
					int by = i >> 3, p = Num_of_Bytes - (i & (Num_of_Bytes - 1)) - 1;
					a.bytes[by] |= 1 << p;
					x -= 1.0;
				}
			}
			if (x > Half || (x >= Half && (a.bytes[(Num_of_Bytes - 1)] & 1))) {
				for (int i = Length_of_Double - 1; i > 0; i--) {
					int by = i >> 3, p = Num_of_Bytes - (i & (Num_of_Bytes - 1)) - 1;
					if (a.bytes[by] & (1 << p))
						a.bytes[by] -= 1 << p;
					else
						a.bytes[by] |= 1 << p;
				}
			}
			return a;
		}
	}
	else {
		while (x < 1.0) {
			x *= Two;
			cnt--;
			if (cnt + Bias <= 0) {
				x /= Two;
				break;
			}
		}
		int tmp = cnt + Bias;
		a.bytes[0] |= tmp >> 4;
		a.bytes[1] = (tmp & Binary_of_Lastfour) << 4;
		if (x >= 1.0)
			x -= 1.0;
		for (int i = Length_of_Exp; i < Length_of_Double; i++) {
			x *= Two;
			if (x >= 1.0) {
				int by = i >> 3, p = Num_of_Bytes - (i & (Num_of_Bytes - 1)) - 1;
				a.bytes[by] |= 1 << p;
				x -= 1.0;
			}
		}
		if (x > Half || (x >= Half && (a.bytes[(Num_of_Bytes - 1)] & 1))) {
			for (int i = Length_of_Double - 1; i > 0; i--) {
				int by = i >> 3, p = Num_of_Bytes - (i & (Num_of_Bytes - 1)) - 1;
				if (a.bytes[by] & (1 << p))
					a.bytes[by] -= 1 << p;
				else
					a.bytes[by] |= 1 << p;
			}
		}
		return a;
	}
}
double trans (struct double_n *a) {
	double x = 0.0;
	if (iszero (a)) {
		double y = 1.0;
		for (int i = Length_of_Exp; i < Length_of_Double; i++) {
			int by = i >> 3, p = Num_of_Bytes - (i & (Num_of_Bytes - 1)) - 1;
			y /= Two;
			if (a->bytes[by] & (1 << p))
				x += y;
		}
		for (int i = 1; i <= (Bias - 1); i++) x /= Two;
	}
	else {
		if (is_inf (a) && !allzero (a,Length_of_Exp))
			return 0.0 / 0.0;
		x = 1.0;
		double y = 1.0;
		for (int i = Length_of_Exp; i < Length_of_Double; i++) {
			int by = i >> 3, p = Num_of_Bytes - (i & (Num_of_Bytes - 1)) - 1;
			y /= Two;
			if (a->bytes[by] & (1 << p))
				x += y;
		}
		int cnt = ((a->bytes[0] & (Half_of_Value - 1)) << 4) + (a->bytes[1] >> 4);
		int tmp = cnt - Bias;
		if (tmp > 0) {
			for (int i = 1; i <= tmp; i++)
				x *= Two;
		}
		else {
			for (int i = 1; i <= -tmp; i++)
				x /= Two;
		}
	}
	if(a->bytes[0] & Half_of_Value) x = -x;
	return x;
}
struct double_n add (struct double_n *a, struct double_n *b) {
	if (is_nan (a))
		return *a;
	if (is_nan (b))
		return *b;
	if (is_inf (a)) {
		if (is_inf (b)) {
			if (sym (a, b)) {
				a->bytes[1] |= 1;
				return *a;
			}
			else
				return *a;
		}
		else
			return *a;
	}
	else {
		if (is_inf (b))
			return *b;
		else {
			struct double_n *c = &dc;
			clear (c);
			int f = 0;
			if (!bigger (a, b))
				Swap (a, b);
			f = a->bytes[0] >> (Num_of_Bytes - 1);
			c->bytes[0] = f << (Num_of_Bytes - 1);
			if (sym (a, b)) {
				if (iszero (a)) {
					int used = 0;
					for (int i = Length_of_Double - 1; i >= Length_of_Exp; i--) {
						int by = i >> 3, p = Num_of_Bytes - (i & (Num_of_Bytes - 1)) - 1;
						int wa = (a->bytes[by] >> p) & 1;
						int wb = (b->bytes[by] >> p) & 1;
						int res = wa - wb - used;
						if (res < 0)
							res += 2, used = 1;
						else
							used = 0;
						c->bytes[by] |= res << p;
					}
					return *c;
				}
				else {
					int m1, m2;
					uc bbytes[Num_of_Bytes << 1];
					uc bytes[Num_of_Bytes << 1];
					for (int i = 0; i < Power_of_Four; i++)
						bbytes[i] = bytes[i] = 0;
					m1 = calexp (a), m2 = calexp (b);
					if (!m2) m2++;
					if (m1 - m2 > Length_of_Double)
						return *a;
					for (int i = Length_of_Exp, j = Length_of_Exp + m1 - m2; i < Length_of_Double; i++, j++) {
						int by = i >> 3, p = Num_of_Bytes - (i & (Num_of_Bytes - 1)) - 1;
						int by2 = j >> 3, p2 = Num_of_Bytes - (j & (Num_of_Bytes - 1)) - 1;
						bbytes[by2] |= ((b->bytes[by] >> p) & 1) << p2;
					}
					if (!iszero (b)) {
						int by2 = (m1 - m2 + (Length_of_Exp - 1)) >> 3, p2 = Num_of_Bytes - ((m1 - m2 + (Length_of_Exp - 1)) & (Num_of_Bytes - 1)) - 1;
						bbytes[by2] |= 1 << p2;
					}
					int used = 0, wa, wb;
					for (int i = (Half_of_Value - 1); i > 0; i--) {
						int by = i >> 3, p = Num_of_Bytes - (i & (Num_of_Bytes - 1)) - 1;
						if (i < Length_of_Double){
							if (i < (Length_of_Exp - 1)) wa = 0;
							else if (i == (Length_of_Exp - 1)) wa = 1;
							else wa = (a->bytes[by] >> p) & 1;
						}
						else wa = 0;
						wb = (bbytes[by] >> p) & 1;
						int res = wa - wb - used;
						if (res < 0)
							res += 2, used = 1;
						else
							used = 0;
						bytes[by] |= res << p;
					}
					int i;
					for (i = 1; i < Half_of_Value; i++) {
						int by = i >> 3, p = Num_of_Bytes - (i & (Num_of_Bytes - 1)) - 1;
						if ((bytes[by] >> p) & 1)
							break;
					}
					if (i == Half_of_Value)
						return *c;
					int ed = i + Length_of_Frac;
					used = 0;
					int by = (ed + 1) >> 3, p = Num_of_Bytes - ((ed + 1) & (Num_of_Bytes - 1)) - 1;
					if ((bytes[by] >> p) & 1) {
						int bj = 0;
						for (int j = ed + 2; j < Half_of_Value; j++) {
							int by2 = j >> 3, p2 = Num_of_Bytes - (j & (Num_of_Bytes - 1)) - 1;
							if ((bytes[by2] >> p2) & 1) {
								bj = 1;
								break;
							}
						}
						if (bj)
							used = 1;
						else {
							int by2 = ed >> 3, p2 = Num_of_Bytes - (ed & (Num_of_Bytes - 1)) - 1;
							if ((bytes[by2] >> p2) & 1)
								used = 1;
						}
					}
					if (used) {
						int j;
						for (j = ed; j >= i - 1; j--) {
							int by2 = j >> 3, p2 = Num_of_Bytes - (j & (Num_of_Bytes - 1)) - 1;
							if ((bytes[by2] >> p2) & 1)
								bytes[by2] -= 1 << p2;
							else {
								bytes[by2] |= 1 << p2;
								used = 0;
								break;
							}
						}
						if (j == i - 1)
							i--;
					}
					int m3 = m1 + (Length_of_Exp - 1) - i;
					if (m3 > 0) {
						c->bytes[0] |= m3 >> 4;
						c->bytes[1] |= (m3 & (Half_of_Value - 1)) << 4;
						for (int k = i+1, l = Length_of_Exp; l < Length_of_Double; k++, l++) {
							int by2 = k >> 3, p2 = Num_of_Bytes - (k & (Num_of_Bytes - 1)) - 1;
							int by3 = l >> 3, p3 = Num_of_Bytes - (l & (Num_of_Bytes - 1)) - 1;
							set (c, by3, p3, (bytes[by2] >> p2) & 1);
						}
					}
					else {
						if (i - m3 - 1 > Length_of_Double)
							return *c;
						int end = m3 + i + (Length_of_Frac - 1);
						int by2 = (end + 1) >> 3, p2 = Num_of_Bytes - ((end + 1) & (Num_of_Bytes - 1)) - 1;
						int used = 0;
						if ((bytes[by2] >> p2) & 1) {
							for (int k = end + 2; k < Half_of_Value; k++) {
								int by3 = k >> 3, p3 = Num_of_Bytes - (k & (Num_of_Bytes - 1)) - 1;
								if ((bytes[by3] >> p3) & 1) {
									used = 1;
									break;
								}
							}
							int by3 = end >> 3, p3 = Num_of_Bytes - (end & (Num_of_Bytes - 1)) - 1;
							if ((bytes[by3] >> p3) & 1)
								used = 1;
						}
						if (used) {
							int k;
							for (k = end; k >= i - 1; k--) {
								int by3 = k >> 3, p3 = Num_of_Bytes - (k & (Num_of_Bytes - 1)) - 1;
								if ((bytes[by3] >> p3) & 1)
									bytes[by3] -= 1 << p3;
								else {
									bytes[by3] |= 1 << p3;
									used = 0;
									break;
								}
							}
							if (k == i - 1)
								i--;
							if (m3 + i < Length_of_Exp) {
								c->bytes[1] |= Power_of_Four;
								return *c;
							}
						}
						for (int k = i, l = Length_of_Exp - m3; l < Length_of_Double; k++, l++) {
							int by2 = k >> 3, p2 = Num_of_Bytes - (k & (Num_of_Bytes - 1)) - 1;
							int by3 = l >> 3, p3 = Num_of_Bytes - (l & (Num_of_Bytes - 1)) - 1;
							set (c, by3, p3, (bytes[by2] >> p2) & 1);
						}
					}
					return *c;
				}
			}
			else {
				if (iszero (a)) {
					int used = 0;
					for (int i = Length_of_Double - 1; i >= Length_of_Exp; i--) {
						int by = i >> 3, p = Num_of_Bytes - (i & (Num_of_Bytes - 1)) - 1;
						int wa = (a->bytes[by] >> p) & 1;
						int wb = (b->bytes[by] >> p) & 1;
						int res = wa + wb + used;
						if (res > 1)
							res -= 2, used = 1;
						else
							used = 0;
						c->bytes[by] |= res << p;
					}
					if (used)
						c->bytes[1] |= Power_of_Four;
					return *c;
				}
				else {
					int m1, m2;
					uc bbytes[Num_of_Bytes << 1];
					uc bytes[Num_of_Bytes << 1];
					for (int i = 0; i < Power_of_Four; i++)
						bbytes[i] = bytes[i] = 0;
					m1 = calexp (a), m2 = calexp (b);
					if (!m2) m2++;
					if (m1 - m2 > Length_of_Double)
						return *a;
					for (int i = Length_of_Exp, j = Length_of_Exp + m1 - m2; i < Length_of_Double; i++, j++) {
						int by = i >> 3, p = Num_of_Bytes - (i & (Num_of_Bytes - 1)) - 1;
						int by2 = j >> 3, p2 = Num_of_Bytes - (j & (Num_of_Bytes - 1)) - 1;
						bbytes[by2] |= ((b->bytes[by] >> p) & 1) << p2;
					}
					if (!iszero (b)) {
						int by2 = (m1 - m2 + (Length_of_Exp - 1)) >> 3, p2 = Num_of_Bytes - ((m1 - m2 + (Length_of_Exp - 1)) & (Num_of_Bytes - 1)) - 1;
						bbytes[by2] |= 1 << p2;
					}
					int used = 0, wa, wb;
					for (int i = (Half_of_Value - 1); i > 0; i--) {
						int by = i >> 3, p = Num_of_Bytes - (i & (Num_of_Bytes - 1)) - 1;
						if (i < Length_of_Double){
							if (i < (Length_of_Exp - 1)) wa = 0;
							else if (i == (Length_of_Exp - 1)) wa = 1;
							else wa = (a->bytes[by] >> p) & 1;
						}
						else wa = 0;
						wb = (bbytes[by] >> p) & 1;
						int res = wa + wb + used;
						if (res > 1)
							res -= 2, used = 1;
						else
							used = 0;
						bytes[by] |= res << p;
					}
					int i;
					for (i = 1; i < Half_of_Value; i++) {
						int by = i >> 3, p = Num_of_Bytes - (i & (Num_of_Bytes - 1)) - 1;
						if ((bytes[by] >> p) & 1)
							break;
					}
					if (i == Half_of_Value)
						return *c;
					int ed = i + Length_of_Frac;
					used = 0;
					int by = (ed + 1) >> 3, p = Num_of_Bytes - ((ed + 1) & (Num_of_Bytes - 1)) - 1;
					if ((bytes[by] >> p) & 1) {
						int bj = 0;
						for (int j = ed + 2; j < Half_of_Value; j++) {
							int by2 = j >> 3, p2 = Num_of_Bytes - (j & (Num_of_Bytes - 1)) - 1;
							if ((bytes[by2] >> p2) & 1) {
								bj = 1;
								break;
							}
						}
						if (bj)
							used = 1;
						else {
							int by2 = ed >> 3, p2 = Num_of_Bytes - (ed & (Num_of_Bytes - 1)) - 1;
							if ((bytes[by2] >> p2) & 1)
								used = 1;
						}
					}
					if (used) {
						int j;
						for (j = ed; j >= i - 1; j--) {
							int by2 = j >> 3, p2 = Num_of_Bytes - (j & (Num_of_Bytes - 1)) - 1;
							if ((bytes[by2] >> p2) & 1)
								bytes[by2] -= 1 << p2;
							else {
								bytes[by2] |= 1 << p2;
								used = 0;
								break;
							}
						}
						if (j == i - 1)
							i--;
					}
					int m3 = m1 + (Length_of_Exp - 1) - i;
					if (m3 > 0) {
						c->bytes[0] |= m3 >> 4;
						c->bytes[1] |= (m3 & (Half_of_Value - 1)) << 4;
						for (int k = i+1, l = Length_of_Exp; l < Length_of_Double; k++, l++) {
							int by2 = k >> 3, p2 = Num_of_Bytes - (k & (Num_of_Bytes - 1)) - 1;
							int by3 = l >> 3, p3 = Num_of_Bytes - (l & (Num_of_Bytes - 1)) - 1;
							set (c, by3, p3, (bytes[by2] >> p2) & 1);
						}
					}
					else {
						if (i - m3 - 1 > Length_of_Double)
							return *c;
						int end = m3 + i + (Length_of_Frac - 1);
						int by2 = (end + 1) >> 3, p2 = Num_of_Bytes - ((end + 1) & (Num_of_Bytes - 1)) - 1;
						int used = 0;
						if ((bytes[by2] >> p2) & 1) {
							for (int k = end + 2; k < Half_of_Value; k++) {
								int by3 = k >> 3, p3 = Num_of_Bytes - (k & (Num_of_Bytes - 1)) - 1;

								if ((bytes[by3] >> p3) & 1) {
									used = 1;
									break;
								}
							}
							int by3 = end >> 3, p3 = Num_of_Bytes - (end & (Num_of_Bytes - 1)) - 1;
							if ((bytes[by3] >> p3) & 1)
								used = 1;
						}
						if (used) {
							int k;
							for (k = end; k >= i - 1; k--) {
								int by3 = k >> 3, p3 = Num_of_Bytes - (k & (Num_of_Bytes - 1)) - 1;
								if ((bytes[by3] >> p3) & 1)
									bytes[by3] -= 1 << p3;
								else {
									bytes[by3] |= 1 << p3;
									used = 0;
									break;
								}
							}
							if (k == i - 1)
								i--;
							if (m3 + i < Length_of_Exp) {
								c->bytes[1] |= Power_of_Four;
								return *c;
							}
						}
						for (int k = i, l = Length_of_Exp - m3; l < Length_of_Double; k++, l++) {
							int by2 = k >> 3, p2 = Num_of_Bytes - (k & (Num_of_Bytes - 1)) - 1;
							int by3 = l >> 3, p3 = Num_of_Bytes - (l & (Num_of_Bytes - 1)) - 1;
							set (c, by3, p3, (bytes[by2] >> p2) & 1);
						}
					}
					return *c;
				}
			}
		}
	}
}
struct double_n sub (struct double_n *a, struct double_n *b) {
	b->bytes[0] ^= Half_of_Value;
	return add (a, b);
}
struct double_n mul (struct double_n *a, struct double_n *b) {
	if (is_nan (a))
		return *a;
	if (is_nan (b))
		return *b;
	if (!bigger (a, b))
		Swap (a, b);
	if (is_inf (a)) {
		if (allzero (b, 1)) {
			a->bytes[1] |= 1;
			return *a;
		}
		else {
			a->bytes[0] ^= (b->bytes[0] >> (Num_of_Bytes - 1)) << (Num_of_Bytes - 1);
			return *a;
		}
	}
	if (iszero (a)) {
		a->bytes[0] &= Half_of_Value;
		for (int i = 1; i < Num_of_Bytes; i++)
			a->bytes[i] = 0;
		return *a;
	}
	else {
		if (iszero (b)) {
			if (allzero (b, Length_of_Exp))
				return *b;
			uc bytes[Num_of_Bytes << 1];
			for (int i = 0; i < Binary_of_Lastfour; i++)
				bytes[i] = 0;
			int m1 = calexp (a), m2 = 1;
			for (int i = Length_of_Exp; i < Length_of_Double; i++)
				for (int j = Length_of_Exp; j < Length_of_Double; j++) {
					int by = i >> 3, p = Num_of_Bytes - (i & (Num_of_Bytes - 1)) - 1;
					int by2 = j >> 3, p2 = Num_of_Bytes - (j & (Num_of_Bytes - 1)) - 1;
					if ((a->bytes[by] >> p) & 1 && (b->bytes[by2] >> p2) & 1) {
						for (int pos = i + j - (Length_of_Exp - 1); pos > 0; pos--) {
							int by3 = pos >> 3, p3 = Num_of_Bytes - (pos & (Num_of_Bytes - 1)) - 1;
							if ((bytes[by3] >> p3) & 1)
								bytes[by3] -= 1 << p3;
							else {
								bytes[by3] |= 1 << p3;
								break;
							}
						}
					}
				}
			for (int i = Length_of_Exp; i < Length_of_Double; i++) {
				int by = i >> 3, p = Num_of_Bytes - (i & (Num_of_Bytes - 1)) - 1;
				if ((b->bytes[by] >> p) & 1) {
					for (int pos = i; pos > 0; pos--) {
						int by3 = pos >> 3, p3 = Num_of_Bytes - (pos & (Num_of_Bytes - 1)) - 1;
						if ((bytes[by3] >> p3) & 1)
							bytes[by3] -= 1 << p3;
						else {
							bytes[by3] |= 1 << p3;
							break;
						}
					}
				}
			}
			int i;
			for (i = 1; i < Half_of_Value; i++) {
				int by = i >> 3, p = Num_of_Bytes - (i & (Num_of_Bytes - 1)) - 1;
				if ((bytes[by] >> p) & 1)
					break;
			}
			int ed = i + Length_of_Frac;
			int used = 0;
			int by = (ed + 1) >> 3, p = Num_of_Bytes - ((ed + 1) & (Num_of_Bytes - 1)) - 1;
			if ((bytes[by] >> p) & 1) {
				int bj = 0;
				for (int j = ed + 2; j < Half_of_Value; j++) {
					int by2 = j >> 3, p2 = Num_of_Bytes - (j & (Num_of_Bytes - 1)) - 1;
					if ((bytes[by2] >> p2) & 1) {
						bj = 1;
						break;
					}
				}
				if (bj)
					used = 1;
				else {
					int by2 = ed >> 3, p2 = Num_of_Bytes - (ed & (Num_of_Bytes - 1)) - 1;
					if ((bytes[by2] >> p2) & 1)
						used = 1;
				}
			}
			if (used) {
				int j;
				for (j = ed; j >= i - 1; j--) {
					int by2 = j >> 3, p2 = Num_of_Bytes - (j & (Num_of_Bytes - 1)) - 1;
					if ((bytes[by2] >> p2) & 1)
						bytes[by2] -= 1 << p2;
					else {
						bytes[by2] |= 1 << p2;
						used = 0;
						break;
					}
				}
				if (j == i - 1)
					i--;
			}
			struct double_n *  c = &dc;
			clear (c);
			c->bytes[0] = (a->bytes[0] & Half_of_Value) ^ (b->bytes[0] & Half_of_Value);
			int m3 = m1 + m2 - Bias + (Length_of_Exp - 1) - i;
			if (m3 > 0) {
				c->bytes[0] |= m3 >> 4;
				c->bytes[1] |= (m3 & (Half_of_Value - 1)) << 4;
				for (int k = i+1, l = Length_of_Exp; l < Length_of_Double; k++, l++) {
					int by2 = k >> 3, p2 = Num_of_Bytes - (k & (Num_of_Bytes - 1)) - 1;
					int by3 = l >> 3, p3 = Num_of_Bytes - (l & (Num_of_Bytes - 1)) - 1;
					set (c, by3, p3, (bytes[by2] >> p2) & 1);
				}
			}
			else {
				if (i - m3 - 1 > Length_of_Double)
					return *c;
				int end = m3 + i + (Length_of_Frac - 1);
				int by2 = (end + 1) >> 3, p2 = Num_of_Bytes - ((end + 1) & (Num_of_Bytes - 1)) - 1;
				int used = 0;
				if ((bytes[by2] >> p2) & 1) {
					for (int k = end + 2; k < Half_of_Value; k++) {
						int by3 = k >> 3, p3 = Num_of_Bytes - (k & (Num_of_Bytes - 1)) - 1;
						if ((bytes[by3] >> p3) & 1) {
							used = 1;
							break;
						}
					}
					int by3 = end >> 3, p3 = Num_of_Bytes - (end & (Num_of_Bytes - 1)) - 1;
					if ((bytes[by3] >> p3) & 1)
						used = 1;
				}
				if (used) {
					int k;
					for (k = end; k >= i - 1; k--) {
						int by3 = k >> 3, p3 = Num_of_Bytes - (k & (Num_of_Bytes - 1)) - 1;
						if ((bytes[by3] >> p3) & 1)
							bytes[by3] -= 1 << p3;
						else {
							bytes[by3] |= 1 << p3;
							used = 0;
							break;
						}
					}
					if (k == i - 1)
						i--;
					if (m3 + i < Length_of_Exp) {
						c->bytes[1] |= Power_of_Four;
						return *c;
					}
				}
				for (int k = i, l = Length_of_Exp - m3; l < Length_of_Double; k++, l++) {
					int by2 = k >> 3, p2 = Num_of_Bytes - (k & (Num_of_Bytes - 1)) - 1;
					int by3 = l >> 3, p3 = Num_of_Bytes - (l & (Num_of_Bytes - 1)) - 1;
					set (c, by3, p3, (bytes[by2] >> p2) & 1);
				}
			}
			return *c;
		}
		else {
			uc bytes[Num_of_Bytes << 1];
			for (int i = 0; i < Binary_of_Lastfour; i++)
				bytes[i] = 0;
			int m1 = calexp (a), m2 = calexp (b);
			bytes[1] |= Power_of_Four;
			for (int i = Length_of_Exp; i < Length_of_Double; i++)
				for (int j = Length_of_Exp; j < Length_of_Double; j++) {
					int by = i >> 3, p = Num_of_Bytes - (i & (Num_of_Bytes - 1)) - 1;
					int by2 = j >> 3, p2 = Num_of_Bytes - (j & (Num_of_Bytes - 1)) - 1;
					if ((a->bytes[by] >> p) & 1 && (b->bytes[by2] >> p2) & 1) {
						for (int pos = i + j - (Length_of_Exp - 1); pos > 0; pos--) {
							int by3 = pos >> 3, p3 = Num_of_Bytes - (pos & (Num_of_Bytes - 1)) - 1;
							if ((bytes[by3] >> p3) & 1)
								bytes[by3] -= 1 << p3;
							else {
								bytes[by3] |= 1 << p3;
								break;
							}
						}
					}
				}
			for (int i = Length_of_Exp; i < Length_of_Double; i++) {
				int by = i >> 3, p = Num_of_Bytes - (i & (Num_of_Bytes - 1)) - 1;
				if ((a->bytes[by] >> p) & 1) {
					for (int pos = i; pos > 0; pos--) {
						int by3 = pos >> 3, p3 = Num_of_Bytes - (pos & (Num_of_Bytes - 1)) - 1;
						if ((bytes[by3] >> p3) & 1)
							bytes[by3] -= 1 << p3;
						else {
							bytes[by3] |= 1 << p3;
							break;
						}
					}
				}
			}
			for (int i = Length_of_Exp; i < Length_of_Double; i++) {
				int by = i >> 3, p = Num_of_Bytes - (i & (Num_of_Bytes - 1)) - 1;
				if ((b->bytes[by] >> p) & 1) {
					for (int pos = i; pos > 0; pos--) {
						int by3 = pos >> 3, p3 = Num_of_Bytes - (pos & (Num_of_Bytes - 1)) - 1;
						if ((bytes[by3] >> p3) & 1)
							bytes[by3] -= 1 << p3;
						else {
							bytes[by3] |= 1 << p3;
							break;
						}
					}
				}
			}
			int i;
			for (i = 1; i < Half_of_Value; i++) {
				int by = i >> 3, p = Num_of_Bytes - (i & (Num_of_Bytes - 1)) - 1;
				if ((bytes[by] >> p) & 1)
					break;
			}
			int ed = i + Length_of_Frac;
			int used = 0;
			int by = (ed + 1) >> 3, p = Num_of_Bytes - ((ed + 1) & (Num_of_Bytes - 1)) - 1;
			if ((bytes[by] >> p) & 1) {
				int bj = 0;
				for (int j = ed + 2; j < Half_of_Value; j++) {
					int by2 = j >> 3, p2 = Num_of_Bytes - (j & (Num_of_Bytes - 1)) - 1;
					if ((bytes[by2] >> p2) & 1) {
						bj = 1;
						break;
					}
				}
				if (bj)
					used = 1;
				else {
					int by2 = ed >> 3, p2 = Num_of_Bytes - (ed & (Num_of_Bytes - 1)) - 1;
					if ((bytes[by2] >> p2) & 1)
						used = 1;
				}
			}
			if (used) {
				int j;
				for (j = ed; j >= i - 1; j--) {
					int by2 = j >> 3, p2 = Num_of_Bytes - (j & (Num_of_Bytes - 1)) - 1;
					if ((bytes[by2] >> p2) & 1)
						bytes[by2] -= 1 << p2;
					else {
						bytes[by2] |= 1 << p2;
						used = 0;
						break;
					}
				}
				if (j == i - 1)
					i--;
			}
			struct double_n *c = &dc;
			clear (c);
			c->bytes[0] = (a->bytes[0] & Half_of_Value) ^ (b->bytes[0] & Half_of_Value);
			int m3 = m1 + m2 - Bias + (Length_of_Exp - 1) - i;
			if (m3 >= Exp_Max) {
				setinf (c);
				return *c;
			}
			if (m3 > 0) {
				c->bytes[0] |= m3 >> 4;
				c->bytes[1] |= (m3 & (Half_of_Value - 1)) << 4;
				for (int k = i+1, l = Length_of_Exp; l < Length_of_Double; k++, l++) {
					int by2 = k >> 3, p2 = Num_of_Bytes - (k & (Num_of_Bytes - 1)) - 1;
					int by3 = l >> 3, p3 = Num_of_Bytes - (l & (Num_of_Bytes - 1)) - 1;
					set (c, by3, p3, (bytes[by2] >> p2) & 1);
				}
			}
			else {
				if (i - m3 - 1 > Length_of_Double)
					return *c;
				int end = m3 + i + (Length_of_Frac - 1);
				int by2 = (end + 1) >> 3, p2 = Num_of_Bytes - ((end + 1) & (Num_of_Bytes - 1)) - 1;
				int used = 0;
				if ((bytes[by2] >> p2) & 1) {
					for (int k = end + 2; k < Half_of_Value; k++) {
						int by3 = k >> 3, p3 = Num_of_Bytes - (k & (Num_of_Bytes - 1)) - 1;
						if ((bytes[by3] >> p3) & 1) {
							used = 1;
							break;
						}
					}
					int by3 = end >> 3, p3 = Num_of_Bytes - (end & (Num_of_Bytes - 1)) - 1;
					if ((bytes[by3] >> p3) & 1)
						used = 1;
				}
				if (used) {
					int k;
					for (k = end; k >= i + m3 - 1; k--) {
						int by3 = k >> 3, p3 = Num_of_Bytes - (k & (Num_of_Bytes - 1)) - 1;
						if ((bytes[by3] >> p3) & 1)
							bytes[by3] -= 1 << p3;
						else {
							bytes[by3] |= 1 << p3;
							used = 0;
							break;
						}
					}
					if (k == i + m3 - 1)
						i--;
					if (i - m3 < Length_of_Exp) {
						c->bytes[1] |= Power_of_Four;
						return *c;
					}
				}
				if (i - m3 >= Length_of_Double)
					return *c;
				for (int k = i, l = Length_of_Exp - m3; l < Length_of_Double; k++, l++) {
					int by2 = k >> 3, p2 = Num_of_Bytes - (k & (Num_of_Bytes - 1)) - 1;
					int by3 = l >> 3, p3 = Num_of_Bytes - (l & (Num_of_Bytes - 1)) - 1;
					set (c, by3, p3, (bytes[by2] >> p2) & 1);
				}
			}
			return *c;
		}
	}
}
struct double_n divi (struct double_n *a, struct double_n *b) {
	if (is_nan (a))
		return *a;
	if (is_nan (b))
		return *b;
	if (is_inf (a)) {
		a->bytes[0] ^= b->bytes[0] & Half_of_Value;
		return *a;
	}
	if (is_inf (b)) {
		a->bytes[0] ^= b->bytes[0] & Half_of_Value;
		a->bytes[0] &= Half_of_Value;
		for (int i = 1; i < Num_of_Bytes; i++)
			a->bytes[i] = 0;
		return *a;
	}
	if (allzero (a, 1)) {
		a->bytes[0] ^= b->bytes[0] & Half_of_Value;
		if (allzero (b, 1)) {
			a->bytes[0] |= (Half_of_Value - 1);
			a->bytes[1] |= (Value_of_Bytes - 1) - Binary_of_Lastfour + 1;
		}
		return *a;
	}
	if (allzero (b, 1)) {
		a->bytes[0] ^= b->bytes[0] & Half_of_Value;
		a->bytes[0] |= (Half_of_Value - 1);
		a->bytes[1] = (Value_of_Bytes - 1) - Binary_of_Lastfour;
		for (int i = 2; i < Num_of_Bytes; i++)
			a->bytes[i] = 0;
		return *a;
	}
	uc abytes[Num_of_Bytes << 1];
	uc bbytes[Num_of_Bytes];
	uc cbytes[Num_of_Bytes << 1];
	for (int i = 0; i < Power_of_Four; i++)
		abytes[i] = 0;
	for (int i = 0; i < Num_of_Bytes; i++)
		bbytes[i] = 0;
	for (int i = 0; i < Power_of_Four; i++)
		cbytes[i] = 0;
	int m1, m2;
	if (iszero (a))
		m1 = 1;
	else {
		m1 = ((a->bytes[0] & (Half_of_Value - 1)) << 4) + (a->bytes[1] >> 4);
		abytes[1] |= Power_of_Four;
	}
	for (int i = Length_of_Exp; i < Length_of_Double; i++) {
		int by = i >> 3, p = Num_of_Bytes - (i & (Num_of_Bytes - 1)) - 1;
		abytes[by] |= ((a->bytes[by] >> p) & 1) << p;
	}
	if (iszero (b))
		m2 = 1;
	else {
		m2 = ((b->bytes[0] & (Half_of_Value - 1)) << 4) + (b->bytes[1] >> 4);
		bbytes[1] |= Power_of_Four;
	}
	for (int i = Length_of_Exp; i < Length_of_Double; i++) {
		int by = i >> 3, p = Num_of_Bytes - (i & (Num_of_Bytes - 1)) - 1;
		bbytes[by] |= ((b->bytes[by] >> p) & 1) << p;
	}
	int pos1, pos2;
	for (pos1 = (Length_of_Exp - 1); pos1 < Length_of_Double; pos1++) {
		int by = pos1 >> 3, p = Num_of_Bytes - (pos1 & (Num_of_Bytes - 1)) - 1;
		if ((abytes[by] >> p) & 1)
			break;
	}
	for (pos2 = (Length_of_Exp - 1); pos2 < Length_of_Double; pos2++) {
		int by = pos2 >> 3, p = Num_of_Bytes - (pos2 & (Num_of_Bytes - 1)) - 1;
		if ((bbytes[by] >> p) & 1)
			break;
	}
	for (int i = pos1; i < Half_of_Value; i++) {
		int used = 1;
		int by = (i - 1) >> 3, p = Num_of_Bytes - ((i - 1) & (Num_of_Bytes - 1)) - 1;
		if (!((abytes[by] >> p) & 1)) {
			int w1, w2;
			for (int j = i, k = pos2; j < Half_of_Value || k < Length_of_Double; j++, k++) {
				int by = j >> 3, p = Num_of_Bytes - (j & (Num_of_Bytes - 1)) - 1;
				int by2 = k >> 3, p2 = Num_of_Bytes - (k & (Num_of_Bytes - 1)) - 1;
				if (j < Half_of_Value) w1 = ((abytes[by] >> p) & 1);
				else w1 = 0;
				if (k < Length_of_Double) w2 = ((bbytes[by2] >> p2) & 1);
				else w2 = 0;
				if (w1 > w2)
					break;
				if (w1 < w2) {
					used = 0;
					break;
				}
			}
		}
		if (used) {
			if (i + Length_of_Double - pos2 > Half_of_Value) {
				int bj = 0;
				int ed = pos2 + Half_of_Value - i;
				int by = ed >> 3, p = Num_of_Bytes - (ed & (Num_of_Bytes - 1)) - 1;
				if ((bbytes[by] >> p) & 1) {
					int by2 = (Half_of_Value - 1) >> 3, p2 = Num_of_Bytes - ((Half_of_Value - 1) & (Num_of_Bytes - 1)) - 1;
					int by3 = (ed - 1) >> 3, p3 = Num_of_Bytes - ((ed - 1) & (Num_of_Bytes - 1)) - 1;
					if (((abytes[by2] >> p2) & 1) ^ ((bbytes[by3] >> p3) & 1))
						bj = 1;
					else {
						for (int j = ed + 1; j < Length_of_Double; j++) {
							int by4 = j >> 3, p4 = Num_of_Bytes - (j & (Num_of_Bytes - 1)) - 1;
							if ((bbytes[by4] >> p4) & 1) {
								bj = 1;
								break;
							}
						}
					}
				}
				for (int j = (Half_of_Value - 1), k = ed - 1; j >= i - 1; j--, k--) {
					int by2 = j >> 3, p2 = Num_of_Bytes - (j & (Num_of_Bytes - 1)) - 1;
					int by3 = k >> 3, p3 = Num_of_Bytes - (k & (Num_of_Bytes - 1)) - 1;
					int w1 = (abytes[by2] >> p2) & 1;
					int w2 = (bbytes[by3] >> p3) & 1;
					int res = w1 - w2 - bj;
					if (res < 0) {
						res += 2; bj = 1;
					}
					else
						bj = 0;
					abytes[by2] &= (Value_of_Bytes - 1) - (1 << p2);
					abytes[by2] |= res << p2;
				}
			}
			else {
				int bj = 0;
				for (int j = i + Length_of_Double - 1 - pos2, k = Length_of_Double - 1; j >= i - 1; j--, k--) {
					int by2 = j >> 3, p2 = Num_of_Bytes - (j & (Num_of_Bytes - 1)) - 1;
					int by3 = k >> 3, p3 = Num_of_Bytes - (k & (Num_of_Bytes - 1)) - 1;
					int w1 = (abytes[by2] >> p2) & 1;
					int w2 = (bbytes[by3] >> p3) & 1;
					int res = w1 - w2 - bj;
					if (res < 0) {
						res += 2; bj = 1;
					}
					else
						bj = 0;
					abytes[by2] &= (Value_of_Bytes - 1) - (1 << p2);
					abytes[by2] |= res << p2;
				}
			}
			int tmp = i - pos1 + (Length_of_Exp - 1);
			int by = tmp >> 3, p = Num_of_Bytes - (tmp & (Num_of_Bytes - 1)) - 1;
			cbytes[by] |= 1 << p;
		}
	}
	if (!(cbytes[1] & Power_of_Four)) {
		m1--;
		for (int i = (Length_of_Exp - 1), j = Length_of_Exp; j < Half_of_Value; i++, j++) {
			int by = i >> 3, p = Num_of_Bytes - (i & (Num_of_Bytes - 1)) - 1;
			int by2 = j >> 3, p2 = Num_of_Bytes - (j & (Num_of_Bytes - 1)) - 1;
			cbytes[by] &= (Value_of_Bytes - 1) - (1 << p);
			cbytes[by] |= ((cbytes[by2] >> p2) & 1) << p;
		}
		cbytes[Binary_of_Lastfour] &= (Value_of_Bytes - 2);
	}
	if (cbytes[Num_of_Bytes] >> (Num_of_Bytes - 1)) {
		int used = 0;
		if (cbytes[(Num_of_Bytes - 1)] & 1)
			used = 1;
		for (int i = (Length_of_Double + 1); i < Half_of_Value; i++) {
			int by = i >> 3, p = Num_of_Bytes - (i & (Num_of_Bytes - 1)) - 1;
			if ((cbytes[by] >> p) & 1) {
				used = 1;
				break;
			}
		}
		if (used) {
			for (int i = Length_of_Double - 1; i > 0; i--) {
				int by = i >> 3, p = Num_of_Bytes - (i & (Num_of_Bytes - 1)) - 1;
				if ((cbytes[by] >> p) & 1)
					cbytes[by] -= 1 << p;
				else {
					cbytes[by] |= 1 << p;
					used = 0;
					break;
				}
			}
		}
	}
	struct double_n *c = &dc;
	clear (c);
	c->bytes[0] = (a->bytes[0] & Half_of_Value) ^ (b->bytes[0] & Half_of_Value);
	int m3 = m1 - m2 + Bias - pos1 + pos2;
	if (cbytes[1] & Power_of_Five)
		m3++;
	if (m3 >= Exp_Max) {
		setinf (c);
		return *c;
	}
	if (m3 > 0) {
		c->bytes[0] |= m3 >> 4;
		c->bytes[1] |= (m3 & (Half_of_Value - 1)) << 4;
		for (int i = Length_of_Exp; i < Length_of_Double; i++) {
			int by = i >> 3, p = Num_of_Bytes - (i & (Num_of_Bytes - 1)) - 1;
			set (c, by, p, (cbytes[by] >> p) & 1);
		}
		return *c;
	}
	else {
		int mov = 1 - m3;
		if (mov > Length_of_Frac)
			return *c;
		int ed = Length_of_Double - mov;
		int by = ed >> 3, p = Num_of_Bytes - (ed & (Num_of_Bytes - 1)) - 1;
		if ((cbytes[by] >> p) & 1) {
			int used = 0;
			int by2 = (ed - 1) >> 3, p2 = Num_of_Bytes - ((ed - 1) & (Num_of_Bytes - 1)) - 1;
			if ((cbytes[by2] >> p2) & 1)
				used = 1;
			for (int i = ed + 1; i < Length_of_Double; i++) {
				int by3 = i >> 3, p3 = Num_of_Bytes - (i & (Num_of_Bytes - 1)) - 1;
				if ((cbytes[by3] >> p3) & 1) {
					used = 1;
					break;
				}
			}
			if (used) {
				for (int i = Length_of_Double - 1; i > 0; i--) {
					int by3 = i >> 3, p3 = Num_of_Bytes - (i & (Num_of_Bytes - 1)) - 1;
					if ((cbytes[by3] >> p3) & 1)
						cbytes[by3] -= 1 << p3;
					else {
						cbytes[by3] |= 1 << p3;
						used = 0;
						break;
					}
				}
			}
			if (cbytes[1] & Power_of_Five) {
				mov--;
				if (!mov) {
					c->bytes[1] |= Power_of_Four;
					return *c;
				}
				else {
					int by = ((Length_of_Exp - 1) + mov) >> 3, p = Num_of_Bytes - (((Length_of_Exp - 1) + mov) & (Num_of_Bytes - 1)) - 1;
					c->bytes[by] |= 1 << p;
					return *c;
				}
			}
		}
		c->bytes[1] |= Power_of_Four;
		for (int i = (Length_of_Exp - 1), j = (Length_of_Exp - 1) + mov; j < Length_of_Double; i++, j++) {
			int by2 = i >> 3, p2 = Num_of_Bytes - (i & (Num_of_Bytes - 1)) - 1;
			int by3 = j >> 3, p3 = Num_of_Bytes - (j & (Num_of_Bytes - 1)) - 1;
			set (c, by3, p3, (cbytes[by2] >> p2) & 1);
		}
		return *c;
	}
}
struct double_n get (int *x) {
	struct double_n v1 = chu (0), tmp, tmp2;
	int f = 0;
	char ch;
	while (*x < n && ((ch = s[*x]) < '0' || ch > '9')) {
		if (ch == '-') f = 1;
		(*x)++;
	}
	while (*x < n && ((ch = s[*x]) >= '0' && ch <= '9')){
		tmp = chu (10);
		tmp = mul (&v1, &tmp);
		tmp2 = chu (ch - '0');
		v1 = add (&tmp, &tmp2);
		(*x)++;
	}
	if ((ch = s[*x]) == '.') {
		(*x)++;
		struct double_n v2 = chu (1);
		while (*x < n && ((ch = s[*x]) >= '0' && ch <= '9')) {
			tmp = chu (10);
			tmp = mul (&v1, &tmp);
			tmp2 = chu (ch - '0');
			v1 = add (&tmp, &tmp2);
			tmp = chu (10);
			v2 = mul (&v2, &tmp);
			(*x)++;
		}
		v1 = divi (&v1, &v2);
	}
	if ((ch = s[*x]) == 'e') {
		(*x)++;
		int f1 = 0;
		if ((ch = s[*x]) == '-') f1 = 1;
		int ex = 1;
		while (*x < n && ((ch = s[*x]) >= '0' && ch <= '9')) {
			ex = ex * 10 + ch - '0';
			(*x)++;
		}
		if (f1)
			ex = -ex;
		if (ex >= 4) {
			setinf (&v1);
		}
		else if (ex <= -4) {
			clear (&v1);
		}
		else {
			if (ex > 0) {
				for (int i = 1; i <= ex; i++) {
					tmp = chu (10);
					v1 = mul (&v1 ,&tmp);
				}
			}
			else if (ex < 0) {
				for (int i = 1; i <= -ex; i++) {
					tmp = chu (10);
					v1 = divi (&v1, &tmp);
				}
			}
		}
	}
	if (f) setneg (&v1);
	return v1;
}
int isopt (char c) {
	return c == '+' || c == '-' || c == '*' || c == '/';
}
char getopt (int *x) {
	char ch = s[*x];
	while (*x < n && !isopt (ch)){
		(*x)++;
		ch = s[*x];
	}
	return ch;
}
struct bignum {
	int sym, len1, len2;
	int inte[Length_of_Bignum];
	int deci[Length_of_Bignum];
}Big;
void set_bignum_zero (struct bignum *a) {
	a->len1 = 1;
	a->len2 = 0;
	a->inte[1] = 0;
}
void mul2 (struct bignum *a) {
	int used = 0;
	for (int i = a->len2; i > 0; i--) {
		a->deci[i] *= 2;
		a->deci[i] += used;
		if (a->deci[i] >= 10) {
			a->deci[i] -= 10;
			used = 1;
			if (a->deci[i] == 0 && i == a->len2)
				a->len2--;
		}
		else
			used = 0;
	}
	for (int i = 1; i <= a->len1; i++) {
		a->inte[i] *= 2;
		a->inte[i] += used;
		if (a->inte[i] >= 10) {
			a->inte[i] -= 10;
			used = 1;
			if (i == a->len1) {
				a->len1++;
				a->inte[a->len1] = 0;
			}
		}
		else
			used = 0;
	}
}
void divi2(struct bignum *a) {
	int used = 0;
	for (int i = a->len1; i > 0; i--) {
		a->inte[i] += used * 10;
		used = a->inte[i] & 1;
		a->inte[i] >>= 1;
		if (a->inte[i] == 0 && i == a->len1 && a->len1 > 1)
			a->len1--;
	}
	if (used && a->len2 == 0) {
		a->len2++;
		a->deci[1] = 0;
	}
	for (int i = 1; i <= a->len2; i++) {
		a->deci[i] += used * 10;
		used = a->deci[i] & 1;
		a->deci[i] >>= 1;
		if (used && i == a->len2) {
			a->len2++;
			a->deci[a->len2] = 0;
		}
	}
}
void add1 (struct bignum *a) {
	if (a->len1 == 0) {
		a->len1 = 1;
		a->inte[1] = 0;
	}
	a->inte[1]++;
}
void Print (struct bignum *a, int pos_of_bit) {
	if (a->sym == 3) {
		puts ("nan");
		return;
	}
	if (a->sym == 2) {
		puts ("inf");
		return;
	}
	if (a->sym == -2) {
		puts ("-inf");
		return;
	}
	if (a->sym == 1)
		printf ("-");
	for (int i = a->len1; i; i--)
		printf ("%d", a->inte[i]);
	if (a->len2 > 0) {
		printf (".");
		if (pos_of_bit == -1) {
			for (int i = 1; i <= a->len2; i++)
				printf ("%d", a->deci[i]);
		}
		else {
			for (int i = 1; i < pos_of_bit; i++) {
				if (i <= a->len2) printf ("%d", a->deci[i]);
				else printf ("0");
			}
			if (pos_of_bit > a->len2)
				printf ("0");
			else {
				int num = a->deci[pos_of_bit];
				if (pos_of_bit + 1 <= a->len2) {
					if (a->deci[pos_of_bit + 1] > 5)
						num++;
					else if (a->deci[pos_of_bit + 1] == 5) {
						for (int i = pos_of_bit + 2; i <= a->len2; i++)
							if (a->deci[i] > 0) {
								num++;
								break;
							}
					}
				}
				printf ("%d", num);
			}
		}
	}
	else {
		printf (".");
		for (int i = 1; i <= pos_of_bit; i++)
			printf ("0");
	}
	printf ("\n");
	return;
}
struct bignum bignum_trans (struct double_n *a) {
	int pow_of_two;
	struct bignum *b = &Big;
	set_bignum_zero (b);
	if (is_nan (a)) {
		b->sym = 3;
		return *b;
	}
	if (is_inf (a)) {
		if (a->bytes[0] >> 7)
			b->sym = -2;
		else b->sym = 2;
		return *b;
	}
	b->sym = a->bytes[0] >> 7;
	if (iszero (a)) {
		pow_of_two = 1 - Bias - 52;
	}
	else {
		pow_of_two = calexp (a) - Bias - 52;
		b->len1 = 1;
		b->inte[1] = 1;
	}
	//Print (b, 5);
	//int cnt1 = 0, cnt2 = 0;
	for (int i = 12; i < 64; i++) {
		mul2 (b);//Print (b, 5);//cnt1++;
		int by = i >> 3, p = Num_of_Bytes - (i & (Num_of_Bytes - 1)) - 1;
		if ((a->bytes[by] >> p) & 1)
			add1 (b);
	}
	//printf ("%d %d %d\n", pow_of_two, b->len1, b->inte[1]);
	if (pow_of_two > 0) {
		for (int i = 1; i <= pow_of_two; i++)
			mul2 (b);
	}
	if (pow_of_two < 0) {
		for (int i = 1; i <= -pow_of_two; i++)
			divi2 (b);//,Print (b, 5); 
	}
	//printf ("%d %d\n",cnt1, cnt2);
	return *b;
}
struct stack{
	int ty;
	struct double_n a;
}st[N];
struct double_n solve () {
	int pre = 0, pos = 0;
	int top = 0;
	while (pos < n) {
		if (s[pos] == ' ') {
			pos++;
		}
		else if (s[pos] == '(') {
			//printf ("5 %d\n", pos);
			top++;
			st[top].ty = 5;
			pre = top;
			pos++;
		}
		else if (s[pos] == ')') {
			//printf ("6 %d\n", pos);
			struct double_n tmp;
			if (top - pre == 1) {
				tmp = st[top].a;
				top = pre - 1;
			}
			else {
				if (st[pre + 2].ty == 1)
					tmp = add (&st[pre + 1].a, &st[pre + 3].a);
				else
					tmp = sub (&st[pre + 1].a, &st[pre + 3].a);
				top = pre - 1;
			}
			if (st[top].ty <= 2 || st[top].ty == 5) {
				top++;
				st[top].ty = 0;
				st[top].a = tmp;
			}
			else {
				if (st[top].ty == 3)
					tmp = mul (&st[top - 1].a, &tmp);
				else
					tmp = divi (&st[top - 1].a, &tmp);
				top--;
				st[top].ty = 0;
				st[top].a = tmp;
			}
			pos++;
		}
		else if (s[pos] == '+') {
			//printf ("1 %d\n", pos);
			if (top - pre > 1) {
				struct double_n tmp;
				if (st[pre + 2].ty == 1)
					tmp = add (&st[pre + 1].a, &st[pre + 3].a);
				else
					tmp = sub (&st[pre + 1].a, &st[pre + 3].a);
				top = pre + 1;
				st[top].ty = 0;
				st[top].a = tmp;
			}
			top++;
			st[top].ty = 1;
			pos++;
		}
		else if (s[pos] == '-') {
			//printf ("2 %d\n", pos);
			if (top - pre > 1) {
				struct double_n tmp;
				if (st[pre + 2].ty == 1)
					tmp = add (&st[pre + 1].a, &st[pre + 3].a);
				else
					tmp = sub (&st[pre + 1].a, &st[pre + 3].a);
				top = pre + 1;
				st[top].ty = 0;
				st[top].a = tmp;
			}
			top++;
			st[top].ty = 2;
			pos++;
		}
		else if (s[pos] == '*') {
			//printf ("3 %d\n", pos);
			top++;
			st[top].ty = 3;
			pos++;
		}
		else if (s[pos] == '/') {
			//printf ("4 %d\n", pos);
			top++;
			st[top].ty = 4;
			pos++;
		}
		else {
			//printf ("0 %d\n", pos);
			struct double_n tmp = get (&pos);
			//double tt = trans (&tmp);
			//printf("%lf\n", tt);
			if (st[top].ty <= 2 || st[top].ty == 5) {
				top++;
				st[top].ty = 0;
				st[top].a = tmp;
			}
			else {
				if (st[top].ty == 3)
					tmp = mul (&st[top - 1].a, &tmp);
				else
					tmp = divi (&st[top - 1].a, &tmp);
				top--;
				st[top].ty = 0;
				st[top].a = tmp;
			}
		}
		//printf ("top= %d %d\n", top, st[top].ty);
		//double tt = trans (&st[top].a);
		//printf ("%lf\n", tt);
	}
	if (top > 1) {
		struct double_n tmp;
		if (st[top - 1].ty == 1)
			tmp = add (&st[top - 2].a, &st[top].a);
		else
			tmp = sub (&st[top - 2].a, &st[top].a);
		top -= 2;
		st[top].ty = 0;
		st[top].a = tmp;
	}
	return st[top].a;
}
int main () {
	//freopen ("double.in", "r", stdin);
	//freopen ("double.out", "w", stdout);
	while (scanf (" %[^\n]", s) != EOF) {
		//printf ("%d", _);
		//double a, b;
		//char c;
		n=strlen (s);
		struct double_n tmp = solve ();
		struct bignum temp = bignum_trans (&tmp);
		Print (&temp, Need_of_Bit);
		/*int pos = 0;
		da = get (&pos);
		c = getopt (&pos);
		db = get (&pos);
		//clear (&dc);
		a = trans (&da);
		b = trans (&db);
		//printf ("%c\n", c);
		//scanf ("%lf %c %lf", &a, &c, &b);
		//da = chu (a), db = chu (b);
		//debug(&da);debug(&db);
		if (c == '+'){
			struct double_n tmp = add (&da, &db);
			struct bignum temp = bignum_trans (&tmp);
			Print (&temp, Need_of_Bit);
			printf ("%.5lf\n", a + b);
			//printf ("%.5lf\n", trans (&tmp));
		}
		if (c == '-'){
			struct double_n tmp = sub (&da, &db);
			struct bignum temp = bignum_trans (&tmp);
			Print (&temp, Need_of_Bit);
			printf ("%.5lf\n", a - b);
			//printf ("%.5lf\n", trans (&tmp));
		}
		if (c == '*'){
			struct double_n tmp = mul (&da, &db);
			struct bignum temp = bignum_trans (&tmp);
			Print (&temp, Need_of_Bit);
			printf ("%.5lf\n", a * b);
			//printf ("%.5lf\n", trans (&tmp));
		}
		if (c == '/'){
			struct double_n tmp = divi (&da, &db);
			struct bignum temp = bignum_trans (&tmp);
			Print (&temp, Need_of_Bit);
			printf ("%.5lf\n", a / b);
			//printf ("%.5lf\n", trans (&tmp));
		}*/
	}
}
//5.1 + 2.3
/*double benchmark_calc(double a, double b, char opt) {
	da = chu (a), db = chu (b);
	if (opt == '+'){
		struct double_n tmp = add (&da, &db);
		return trans (&tmp);
	}
	if (opt == '-'){
		struct double_n tmp = sub (&da, &db);
		return trans (&tmp);
	}
	if (opt == '*'){
		struct double_n tmp = mul (&da, &db);
		puts ("bbb");
		return trans (&tmp);
	}
	if (opt == '/'){
		struct double_n tmp = divi (&da, &db);
		return trans (&tmp);
	}
}*/
