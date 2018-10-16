#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//#define uc unsigned char
typedef unsigned char uc;
#define ull unsigned long long
#define NUM_OF_BYTES 8
const int Bias = (1 << 10) - 1;
const int Exp_Max = (1 << 11) - 1;
struct double_n{
	unsigned char bytes[8];
	
}da,db,dc;
void debug (struct double_n *a) {
	for (int i = 0; i < 8; i++) {
		for (int j = 7; j >= 0; j--)
			printf ("%d", (a->bytes[i] >> j) & 1);
		printf (" ");
	}
	puts ("");
}
void clear (struct double_n *a) {
	for (int i = 0; i < 8; i++)
		a->bytes[i] = 0;
}
void set (struct double_n *a, int by, int p, int v) {
	a->bytes[by] &= 255 - (1 << p);
	a->bytes[by] |= v << p;
}
void setinf (struct double_n *a) {
	a->bytes[0] |= 127;
	a->bytes[1] = 255 - 15;
	for (int i = 2; i < 8; i++)
		a->bytes[i] = 0;
}
void lmov (struct double_n *a, int mov) {
	int i = 12;
	for (int j = 12 + mov; j < 64; i++, j++) {
		int by = i >> 3, p = 8 - (i & 7) - 1;
		int by2 = j >> 3, p2 = 8 - (j & 7) - 1;
		set (a, by, p, (a->bytes[by2] >> p2) & 1);
	}
	for (int j = i + 1; j < 64; j++) {
		int by = j >> 3, p = 8 - (j & 7) - 1;
		set (a, by, p, 0);
	}
}
int rmov (struct double_n *a, int mov, int w) {
	if (mov >= 54) {
		a->bytes[1] &= 255 - 15;
		for (int i = 2; i < 8; i++)
			a->bytes[i] = 0;
		return 0;
	}
	int by = (64 - mov) >> 3, p = 8 - ((64 - mov) & 7) - 1;
	int by2 = (63 - mov) >> 3, p2 = 8 - ((63 - mov) & 7) - 1;
	int bj = 0;
	for (int i = 65 - mov; i < 64; i++) {
		int by3 = i >> 3, p3 = 8 - (i & 7) - 1;
		if ((a->bytes[by3] >> p3) & 1) {
			bj = 1;
			break;
		}
	}
	int used = 0;
	if ((a->bytes[by] >> p) & 1) {
		if (bj)
			used = 1;
		else {
			if (((a->bytes[by2] >> p2) & 1) ^ w)
				used = 1;
		}
	}
	if (used) {
		for (int i = 63 - mov; i >= 12; i--) {
			by = i >> 3, p = 8 - (i & 7) - 1;
			if ((a->bytes[by] >> p) & 1)
				set (a, by, p, 0);
			else {
				set (a, by, p, 1);
				used = 0;
				break;
			}
		}
	}
	if (used) {
		if (mov == 1) {
			a->bytes[1] &= 255 - 15;
			for (int i = 2; i < 8; i++)
				a->bytes[i] = 0;
			return 1;
		}
		else {
			a->bytes[1] &= 255 - 15;
			for (int i = 2; i < 8; i++)
				a->bytes[i] = 0;
			by = (10 + mov) >> 3, p = 8 - ((10 + mov) & 7) - 1;
			set (a, by, p, 1);
			return 0;
		}
	}
	else {
		int i = 63;
		for (int j = 63 - mov; j >= 12; i--, j--) {
			by = i >> 3, p = 8 - (i & 7) - 1;
			by2 = j >> 3, p2 = 8 - (j & 7) - 1;
			set (a, by, p, (a->bytes[by2] >> p2) & 1);
		}
		by = i >> 3, p = 8 - (i & 7) - 1;
		set (a, by, p, 1);
		for (int j = i - 1; j >= 12; j--) {
			by = j >> 3, p = 8 - (j & 7) - 1;
			set (a, by, p, 0);
		}
		return 0;
	}
}
void Swap (struct double_n *a, struct double_n *b) {
	for (int i = 0; i < 8; i++) {
		uc tmp = a->bytes[i];
		a->bytes[i] = b->bytes[i];
		b->bytes[i] = tmp;
	}
}
int iszero (struct double_n *a) {
	return (a->bytes[0] & 127) == 0 && (a->bytes[1] >> 4) == 0;
}
int is_inf (struct double_n *a) {
	return (a->bytes[0] & 127) == 127 && ((a->bytes[1] >> 4) & 15) == 15;
}
int is_nan (struct double_n *a) {
	if (is_inf (a) == 0)
		return 0;
	if (a->bytes[1] & 15)
		return 1;
	for (int i = 2; i < 8; i++)
		if (a->bytes[i])
			return 1;
	return 0;
}
int sym (struct double_n *a, struct double_n *b) {
	return (a->bytes[0] >> 7) ^ (b->bytes[0] >> 7);
}
int bigger (struct double_n *a, struct double_n *b) {
	if ((a->bytes[0] & 127) > (b->bytes[0] & 127)) return 1;
	if ((a->bytes[0] & 127) < (b->bytes[0] & 127)) return 0;
	for (int i = 1; i < 8; i++) {
		if (a->bytes[i] > b->bytes [i]) return 1;
		if (a->bytes[i] < b->bytes [i]) return 0;
	}
	return 1;
}
int allzero (struct double_n *a, int b) {
	for (int i = b; i < 64; i++) {
		int by = i >> 3, p = 8 - (i & 7) - 1;
		if ((a->bytes[by] >> p) & 1)
			return 0;
	}
	return 1;
}
int calexp (struct double_n *a) {
	int mor = 0;
	for (int i = 1; i < 12; i++) {
		int by = i >> 3, p = 8 - (i & 7) - 1;
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
		a.bytes[0] = 128;
		x = -x;
	}
	int cnt = 0;
	if (x >= 1.0) {
		while (x >= 2.0) {
			x /= 2.0;
			cnt++;
			if (cnt + Bias >= Exp_Max)
				break;
		}
		int tmp = cnt + Bias;
		if (tmp >= Exp_Max) {
			a.bytes[0] |= 127;
			a.bytes[1] = 255 - 15;
			return a;
		}
		else {
			a.bytes[0] |= tmp >> 4;
			a.bytes[1] = (tmp & 15) << 4;
			x -= 1.0;
			for (int i = 12; i < 64; i++) {
				x *= 2.0;
				if (x >= 1.0) {
					int by = i >> 3, p = 8 - (i & 7) - 1;
					a.bytes[by] |= 1 << p;
					x -= 1.0;
				}
			}
			if (x > 0.5 || (x >= 0.5 && (a.bytes[7] & 1))) {
				for (int i = 63; i > 0; i--) {
					int by = i >> 3, p = 8 - (i & 7) - 1;
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
			x *= 2.0;
			cnt--;
			if (cnt + Bias <= 0) {
				x /= 2.0;
				break;
			}
		}
		int tmp = cnt + Bias;
		a.bytes[0] |= tmp >> 4;
		a.bytes[1] = (tmp & 15) << 4;
		if (x >= 1.0)
			x -= 1.0;
		for (int i = 12; i < 64; i++) {
			x *= 2.0;
			if (x >= 1.0) {
				int by = i >> 3, p = 8 - (i & 7) - 1;
				a.bytes[by] |= 1 << p;
				x -= 1.0;
			}
		}
		if (x > 0.5 || (x >= 0.5 && (a.bytes[7] & 1))) {
			for (int i = 63; i > 0; i--) {
				int by = i >> 3, p = 8 - (i & 7) - 1;
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
		for (int i = 12; i < 64; i++) {
			int by = i >> 3, p = 8 - (i & 7) - 1;
			y /= 2.0;
			if (a->bytes[by] & (1 << p))
				x += y;
		}
		for (int i = 1; i <= 1022; i++) x /= 2.0;
	}
	else {
		if (is_inf (a) && !allzero (a,12))
			return 0.0 / 0.0;
		x = 1.0;
		double y = 1.0;
		for (int i = 12; i < 64; i++) {
			int by = i >> 3, p = 8 - (i & 7) - 1;
			y /= 2.0;
			if (a->bytes[by] & (1 << p))
				x += y;
		}
		int cnt = ((a->bytes[0] & 127) << 4) + (a->bytes[1] >> 4);
		int tmp = cnt - Bias;
		if (tmp > 0) {
			for (int i = 1; i <= tmp; i++)
				x *= 2.0;
		}
		else {
			for (int i = 1; i <= -tmp; i++)
				x /= 2.0;
		}
	}
	if(a->bytes[0] & 128) x = -x;
	return x;
}
struct double_n add (struct double_n *a, struct double_n *b) {
	//debug (a), debug (b);
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
			//a.debug();b.debug();
			if (!bigger (a, b))
				Swap (a, b);
			//a.debug();b.debug();
			f = a->bytes[0] >> 7;
			c->bytes[0] = f << 7;
			if (sym (a, b)) {
				if (iszero (a)) {
					int used = 0;
					for (int i = 63; i >= 12; i--) {
						int by = i >> 3, p = 8 - (i & 7) - 1;
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
					uc bbytes[NUM_OF_BYTES << 1];
					uc bytes[NUM_OF_BYTES << 1];
					for (int i = 0; i < 16; i++)
						bbytes[i] = bytes[i] = 0;
					m1 = calexp (a), m2 = calexp (b);
					if (!m2) m2++;
					if (m1 - m2 > 64)
						return *a;
					for (int i = 12, j = 12 + m1 - m2; i < 64; i++, j++) {
						int by = i >> 3, p = 8 - (i & 7) - 1;
						int by2 = j >> 3, p2 = 8 - (j & 7) - 1;
						bbytes[by2] |= ((b->bytes[by] >> p) & 1) << p2;
					}
					if (!iszero (b)) {
						int by2 = (m1 - m2 + 11) >> 3, p2 = 8 - ((m1 - m2 + 11) & 7) - 1;
						bbytes[by2] |= 1 << p2;
					}
					int used = 0, wa, wb;
					for (int i = 127; i > 0; i--) {
						int by = i >> 3, p = 8 - (i & 7) - 1;
						if (i < 64){
							if (i < 11) wa = 0;
							else if (i == 11) wa = 1;
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
					/*for (int i = 0; i < 16; i++) {
						for (int j = 7; j >= 0; j--)
							printf ("%d", (bbytes[i] >> j) & 1);
						printf (" ");
					}
					puts ("");
					for (int i = 0; i < 16; i++) {
						for (int j = 7; j >= 0; j--)
							printf ("%d", (bytes[i] >> j) & 1);
						printf (" ");
					}
					puts ("");*/
					int i;
					for (i = 1; i < 128; i++) {
						int by = i >> 3, p = 8 - (i & 7) - 1;
						if ((bytes[by] >> p) & 1)
							break;
					}
					if (i == 128)
						return *c;
					int ed = i + 52;
					used = 0;
					int by = (ed + 1) >> 3, p = 8 - ((ed + 1) & 7) - 1;
					if ((bytes[by] >> p) & 1) {
						int bj = 0;
						for (int j = ed + 2; j < 128; j++) {
							int by2 = j >> 3, p2 = 8 - (j & 7) - 1;
							if ((bytes[by2] >> p2) & 1) {
								bj = 1;
								break;
							}
						}
						if (bj)
							used = 1;
						else {
							int by2 = ed >> 3, p2 = 8 - (ed & 7) - 1;
							if ((bytes[by2] >> p2) & 1)
								used = 1;
						}
					}
					if (used) {
						int j;
						for (j = ed; j >= i - 1; j--) {
							int by2 = j >> 3, p2 = 8 - (j & 7) - 1;
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
					int m3 = m1 + 11 - i;
					if (m3 > 0) {
						c->bytes[0] |= m3 >> 4;
						c->bytes[1] |= (m3 & 127) << 4;
						for (int k = i+1, l = 12; l < 64; k++, l++) {
							int by2 = k >> 3, p2 = 8 - (k & 7) - 1;
							int by3 = l >> 3, p3 = 8 - (l & 7) - 1;
							set (c, by3, p3, (bytes[by2] >> p2) & 1);
						}
					}
					else {
						if (i - m3 - 1 > 64)
							return *c;
						int end = m3 + i + 51;
						int by2 = (end + 1) >> 3, p2 = 8 - ((end + 1) & 7) - 1;
						int used = 0;
						if ((bytes[by2] >> p2) & 1) {
							for (int k = end + 2; k < 128; k++) {
								int by3 = k >> 3, p3 = 8 - (k & 7) - 1;
								if ((bytes[by3] >> p3) & 1) {
									used = 1;
									break;
								}
							}
							int by3 = end >> 3, p3 = 8 - (end & 7) - 1;
							if ((bytes[by3] >> p3) & 1)
								used = 1;
						}
						if (used) {
							int k;
							for (k = end; k >= i - 1; k--) {
								int by3 = k >> 3, p3 = 8 - (k & 7) - 1;
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
							if (m3 + i < 12) {
								c->bytes[1] |= 16;
								return *c;
							}
						}
						for (int k = i, l = 12 - m3; l < 64; k++, l++) {
							int by2 = k >> 3, p2 = 8 - (k & 7) - 1;
							int by3 = l >> 3, p3 = 8 - (l & 7) - 1;
							set (c, by3, p3, (bytes[by2] >> p2) & 1);
						}
					}
					return *c;
					/*if (iszero (b)) {
						int mor = calexp (a);
						if (mor - 1 > 52)
							return *a;
						else {
							int ep = 65 - mor;
							int by = ep >> 3, p = 8 - (ep & 7) - 1;
							int used = 0;
							if ((b->bytes[by] >> p) & 1) {
								if (allzero (b, ep + 1)) {
									int by2 = (ep - 1) >> 3, p2 = 8 - ((ep - 1) & 7) - 1;
									if ((a->bytes[63] & 1) ^ ((b->bytes[by2] >> p2) & 1))
										used = 1;
								}
								else
									used = 1;
							}
							int i = 63;
							for (int j = ep - 1; j >= 12; i--, j--) {
								int by = i >> 3, p = 8 - (i & 7) - 1;
								int by2 = j >> 3, p2 = 8 - (j & 7) - 1;
								int wa = (a->bytes[by] >> p) & 1;
								int wb = (b->bytes[by2] >> p2) & 1;
								int res = wa - wb - used;
								if (res < 0)
									res += 2, used = 1;
								else
									used = 0;
								c->bytes[by] |= res << p;
							}
							if (used) {
								for (int j = i; j >= 12; j--) {
									int by = j >> 3, p = 8 - (j & 7) - 1;
									if ((a->bytes[by] >> p) & 1) {
										used = 0;
										break;
									}
									else
										c->bytes[by] |= 1 << p;
								}
								if (used) {
									mor--;
									lmov (c, 1);
								}
							}
							c->bytes[0] |= mor >> 4;
							c->bytes[1] |= (mor & 15) << 4;
							return *c;
						}
					}
					int m1 = calexp (a), m2 = calexp (b);
					if (m1 == m2) {
						int used = 0;
						for (int i = 63; i >= 12; i--) {
							int by = i >> 3, p = 8 - (i & 7) - 1;
							int wa = (a->bytes[by] >> p) & 1;
							int wb = (b->bytes[by] >> p) & 1;
							int res = wa - wb - used;
							if (res < 0)
								res += 2, used = 1;
							else
								used = 0;
							c->bytes[by] |= res << p;
						}
						int i = 12;
						while (i < 64) {
							int by = i >> 3, p = 8 - (i & 7) - 1;
							if ((c->bytes[by] >> p) & 1)
								break;
							i++;
						}
						if (i == 64) {
							c->bytes[0] &= 128;
							c->bytes[1] = 0;
							return *c;
						}
						else {
							int mov = i - 11;
							if (m1 <= mov) {
								lmov (c, m1 - 1);
								c->bytes[0] &= 128;
								c->bytes[1] &= 15;
							}
							else {
								lmov (c, mov);
								m1 -= mov;
								c->bytes[0] &= 128;
								c->bytes[0] |= m1 >> 4;
								c->bytes[1] &= 15;
								c->bytes[1] |= (m1 & 15) << 4;
							}
							return *c;
						}
					}
					else {
						printf ("%d %d\n",&m1, &m2);
						int used = rmov (b, m1 - m2, a->bytes[7] & 1);
						debug (b);
						if (used) {
							int i;
							for (i = 12; i < 64; i++) {
								int by = i >> 3, p = 8 - (i & 7) - 1;
								if ((a->bytes[by] >> p) & 1)
									break;
							}
							if (i == 64) {
								c->bytes[0] &= 128;
								for (int j = 1; j < 8; j++)
									c->bytes[j] = 0;
								return *c;
							}
							else {
								c->bytes[0] &= 128;
								c->bytes[0] |= a->bytes[0] & 127;
								for (int j = 1; j < 8; j++)
									c->bytes[j] = a->bytes[j];
								int e = m1 - i + 11;
								if (e <= 0) {
									c->bytes[0] &= 128;
									c->bytes[1] &= 15;
									lmov (c, m1 - 1);
								}
								else {
									c->bytes[0] &= 128;
									c->bytes[0] |= e >> 4;
									c->bytes[1] &= 15;
									c->bytes[1] |= (e & 15) << 4;
									lmov (c, i - 11);
								}
								return *c;
							}
						}
						else {
							int used = 0;
							for (int i = 63; i >= 12; i--) {
								int by = i >> 3, p = 8 - (i & 7) - 1;
								int wa = (a->bytes[by] >> p) & 1;
								int wb = (b->bytes[by] >> p) & 1;
								int res = wa - wb - used;
								if (res < 0)
									res += 2, used = 1;
								else
									used = 0;
								c->bytes[by] |= res << p;
							}
							debug (c);
							if (used) {
								int i;
								for (i = 12; i < 64; i++) {
									int by = i >> 3, p = 8 - (i & 7) - 1;
									if ((c->bytes[by] >> p) & 1)
										break;
								}
								if (i == 64) {
									c->bytes[0] &= 128;
									for (int j = 1; j < 8; j++)
										c->bytes[j] = 0;
								}
								else {
									int e = m1 - i + 11;
									if (e <= 0) {
										c->bytes[0] &= 128;
										c->bytes[1] &= 15;
										lmov (c, m1 - 1);
									}
									else {
										c->bytes[0] &= 128;
										c->bytes[0] |= e >> 4;
										c->bytes[1] &= 15;
										c->bytes[1] |= (e & 15) << 4;
										lmov (c, i - 11);
									}
								}
							}
							else {
								c->bytes[0] &= 128;
								c->bytes[0] |= m1 >> 4;
								c->bytes[1] &= 15;
								c->bytes[1] |= (m1 & 15) << 4;
							}
							return *c;
						}
					}*/
				}
			}
			else {
				if (iszero (a)) {
					int used = 0;
					for (int i = 63; i >= 12; i--) {
						int by = i >> 3, p = 8 - (i & 7) - 1;
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
						c->bytes[1] |= 16;
					return *c;
				}
				else {
					int m1, m2;
					uc bbytes[NUM_OF_BYTES << 1];
					uc bytes[NUM_OF_BYTES << 1];
					for (int i = 0; i < 16; i++)
						bbytes[i] = bytes[i] = 0;
					m1 = calexp (a), m2 = calexp (b);
					if (!m2) m2++;
					if (m1 - m2 > 64)
						return *a;
					for (int i = 12, j = 12 + m1 - m2; i < 64; i++, j++) {
						int by = i >> 3, p = 8 - (i & 7) - 1;
						int by2 = j >> 3, p2 = 8 - (j & 7) - 1;
						bbytes[by2] |= ((b->bytes[by] >> p) & 1) << p2;
					}
					if (!iszero (b)) {
						int by2 = (m1 - m2 + 11) >> 3, p2 = 8 - ((m1 - m2 + 11) & 7) - 1;
						bbytes[by2] |= 1 << p2;
					}
					int used = 0, wa, wb;
					for (int i = 127; i > 0; i--) {
						int by = i >> 3, p = 8 - (i & 7) - 1;
						if (i < 64){
							if (i < 11) wa = 0;
							else if (i == 11) wa = 1;
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
					/*for (int i = 0; i < 16; i++) {
						for (int j = 7; j >= 0; j--)
							printf ("%d", (bbytes[i] >> j) & 1);
						printf (" ");
					}
					puts ("");
					for (int i = 0; i < 16; i++) {
						for (int j = 7; j >= 0; j--)
							printf ("%d", (bytes[i] >> j) & 1);
						printf (" ");
					}
					puts ("");*/
					int i;
					for (i = 1; i < 128; i++) {
						int by = i >> 3, p = 8 - (i & 7) - 1;
						if ((bytes[by] >> p) & 1)
							break;
					}
					if (i == 128)
						return *c;
					int ed = i + 52;
					used = 0;
					int by = (ed + 1) >> 3, p = 8 - ((ed + 1) & 7) - 1;
					if ((bytes[by] >> p) & 1) {
						int bj = 0;
						for (int j = ed + 2; j < 128; j++) {
							int by2 = j >> 3, p2 = 8 - (j & 7) - 1;
							if ((bytes[by2] >> p2) & 1) {
								bj = 1;
								break;
							}
						}
						if (bj)
							used = 1;
						else {
							int by2 = ed >> 3, p2 = 8 - (ed & 7) - 1;
							if ((bytes[by2] >> p2) & 1)
								used = 1;
						}
					}
					if (used) {
						int j;
						for (j = ed; j >= i - 1; j--) {
							int by2 = j >> 3, p2 = 8 - (j & 7) - 1;
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
					int m3 = m1 + 11 - i;
					if (m3 > 0) {
						c->bytes[0] |= m3 >> 4;
						c->bytes[1] |= (m3 & 127) << 4;
						for (int k = i+1, l = 12; l < 64; k++, l++) {
							int by2 = k >> 3, p2 = 8 - (k & 7) - 1;
							int by3 = l >> 3, p3 = 8 - (l & 7) - 1;
							set (c, by3, p3, (bytes[by2] >> p2) & 1);
						}
					}
					else {
						if (i - m3 - 1 > 64)
							return *c;
						int end = m3 + i + 51;
						int by2 = (end + 1) >> 3, p2 = 8 - ((end + 1) & 7) - 1;
						int used = 0;
						if ((bytes[by2] >> p2) & 1) {
							for (int k = end + 2; k < 128; k++) {
								int by3 = k >> 3, p3 = 8 - (k & 7) - 1;

								if ((bytes[by3] >> p3) & 1) {
									used = 1;
									break;
								}
							}
							int by3 = end >> 3, p3 = 8 - (end & 7) - 1;
							if ((bytes[by3] >> p3) & 1)
								used = 1;
						}
						if (used) {
							int k;
							for (k = end; k >= i - 1; k--) {
								int by3 = k >> 3, p3 = 8 - (k & 7) - 1;
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
							if (m3 + i < 12) {
								c->bytes[1] |= 16;
								return *c;
							}
						}
						for (int k = i, l = 12 - m3; l < 64; k++, l++) {
							int by2 = k >> 3, p2 = 8 - (k & 7) - 1;
							int by3 = l >> 3, p3 = 8 - (l & 7) - 1;
							set (c, by3, p3, (bytes[by2] >> p2) & 1);
						}
					}
					return *c;
					/*if (iszero (b)) {
						int mor = calexp (a);
						if (mor - 1 > 52)
							return *a;
						else {
							int ep = 65 - mor;
							int by = ep >> 3, p = 8 - (ep & 7) - 1;
							int used = 0;
							if ((b->bytes[by] >> p) & 1) {
								if (allzero (b, ep + 1)) {
									int by2 = (ep - 1) >> 3, p2 = 8 - ((ep - 1) & 7) - 1;
									if ((a->bytes[63] & 1) ^ ((b->bytes[by2] >> p2) & 1))
										used = 1;
								}
								else
									used = 1;
							}
							int i = 63;
							for (int j = ep - 1; j >= 12; i--, j--) {
								int by = i >> 3, p = 8 - (i & 7) - 1;
								int by2 = j >> 3, p2 = 8 - (j & 7) - 1;
								int wa = (a->bytes[by] >> p) & 1;
								int wb = (b->bytes[by2] >> p2) & 1;
								int res = wa + wb + used;
								if (res > 1)
									res -= 2, used = 1;
								else
									used = 0;
								c->bytes[by] |= res << p;
							}
							if (used) {
								for (int j = i; j >= 12; j--) {
									int by = j >> 3, p = 8 - (j & 7) - 1;
									if (!((a->bytes[by] >> p) & 1)) {
										c->bytes[by] |= 1 << p;
										used = 0;
										break;
									}
								}
								if (used) {
									mor++;
									if (mor == Exp_Max) {
										setinf (c);
										return *c;
									}
									rmov (c, 1, 0);
									c->bytes[1] &= 255 - 8;
								}
							}
							c->bytes[0] |= mor >> 4;
							c->bytes[1] |= (mor & 15) << 4;
							return *c;
						}
					}
					int m1 = calexp (a), m2 = calexp (b);
					if (m1 == m2) {
						int used = 0;
						for (int i = 63; i >= 12; i--) {
							int by = i >> 3, p = 8 - (i & 7) - 1;
							int wa = (a->bytes[by] >> p) & 1;
							int wb = (b->bytes[by] >> p) & 1;
							int res = wa + wb + used;
							if (res > 1)
								res -= 2, used = 1;
							else
								used = 0;
							c->bytes[by] |= res << p;
						}
						rmov (c, 1, 0);
						c->bytes[1] &= 255 - 8;
						c->bytes[1] |= used << 3;
						m1++;
						if (m1 == Exp_Max) {
							setinf (c);
							return *c;
						}
						c->bytes[0] |= m1 >> 4;
						c->bytes[1] |= (m1 & 15) << 4;
						return *c;
					}
					else {
						int used = rmov (b, m1 - m2, a->bytes[7] & 1);
						//debug (b);
						if (used) {
							rmov (c, 1, 0);
							c->bytes[1] &= 255 - 8;
							c->bytes[1] |= used << 3;
							m1++;
							if (m1 == Exp_Max) {
								setinf (c);
								return *c;
							}
							c->bytes[0] |= m1 >> 4;
							c->bytes[1] |= (m1 & 15) << 4;
							return *c;
						}
						else {
							int used = 0;
							for (int i = 63; i >= 12; i--) {
								int by = i >> 3, p = 8 - (i & 7) - 1;
								int wa = (a->bytes[by] >> p) & 1;
								int wb = (b->bytes[by] >> p) & 1;
								int res = wa + wb + used;
								if (res > 1)
									res -= 2, used = 1;
								else
									used = 0;
								c->bytes[by] |= res << p;
							}
							if (used) {
								rmov (c, 1, 0);
								c->bytes[1] &= 255 - 8;
								m1++;
								if (m1 == Exp_Max) {
									setinf (c);
									return *c;
								}
							}
							c->bytes[0] |= m1 >> 4;
							c->bytes[1] |= (m1 & 15) << 4;							
							return *c;
						}
					}*/
				}
			}
		}
	}
}
struct double_n sub (struct double_n *a, struct double_n *b) {
	b->bytes[0] ^= 128;
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
			a->bytes[0] ^= (b->bytes[0] >> 7) << 7;
			return *a;
		}
	}
	if (iszero (a)) {
		a->bytes[0] &= 128;
		for (int i = 1; i < 8; i++)
			a->bytes[i] = 0;
		return *a;
	}
	else {
		if (iszero (b)) {
			if (allzero (b, 12))
				return *b;
			uc bytes[NUM_OF_BYTES << 1];
			for (int i = 0; i < 15; i++)
				bytes[i] = 0;
			int m1 = calexp (a), m2 = 1;
			for (int i = 12; i < 64; i++)
				for (int j = 12; j < 64; j++) {
					int by = i >> 3, p = 8 - (i & 7) - 1;
					int by2 = j >> 3, p2 = 8 - (j & 7) - 1;
					if ((a->bytes[by] >> p) & 1 && (b->bytes[by2] >> p2) & 1) {
						for (int pos = i + j - 11; pos > 0; pos--) {
							int by3 = pos >> 3, p3 = 8 - (pos & 7) - 1;
							if ((bytes[by3] >> p3) & 1)
								bytes[by3] -= 1 << p3;
							else {
								bytes[by3] |= 1 << p3;
								break;
							}
						}
					}
				}
			for (int i = 12; i < 64; i++) {
				int by = i >> 3, p = 8 - (i & 7) - 1;
				if ((b->bytes[by] >> p) & 1) {
					for (int pos = i; pos > 0; pos--) {
						int by3 = pos >> 3, p3 = 8 - (pos & 7) - 1;
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
			for (i = 1; i < 128; i++) {
				int by = i >> 3, p = 8 - (i & 7) - 1;
				if ((bytes[by] >> p) & 1)
					break;
			}
			int ed = i + 52;
			int used = 0;
			int by = (ed + 1) >> 3, p = 8 - ((ed + 1) & 7) - 1;
			if ((bytes[by] >> p) & 1) {
				int bj = 0;
				for (int j = ed + 2; j < 128; j++) {
					int by2 = j >> 3, p2 = 8 - (j & 7) - 1;
					if ((bytes[by2] >> p2) & 1) {
						bj = 1;
						break;
					}
				}
				if (bj)
					used = 1;
				else {
					int by2 = ed >> 3, p2 = 8 - (ed & 7) - 1;
					if ((bytes[by2] >> p2) & 1)
						used = 1;
				}
			}
			if (used) {
				int j;
				for (j = ed; j >= i - 1; j--) {
					int by2 = j >> 3, p2 = 8 - (j & 7) - 1;
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
			c->bytes[0] = (a->bytes[0] & 128) ^ (b->bytes[0] & 128);
			int m3 = m1 + m2 - Bias + 11 - i;
			if (m3 > 0) {
				c->bytes[0] |= m3 >> 4;
				c->bytes[1] |= (m3 & 127) << 4;
				for (int k = i+1, l = 12; l < 64; k++, l++) {
					int by2 = k >> 3, p2 = 8 - (k & 7) - 1;
					int by3 = l >> 3, p3 = 8 - (l & 7) - 1;
					set (c, by3, p3, (bytes[by2] >> p2) & 1);
				}
			}
			else {
				if (i - m3 - 1 > 64)
					return *c;
				int end = m3 + i + 51;
				int by2 = (end + 1) >> 3, p2 = 8 - ((end + 1) & 7) - 1;
				int used = 0;
				if ((bytes[by2] >> p2) & 1) {
					for (int k = end + 2; k < 128; k++) {
						int by3 = k >> 3, p3 = 8 - (k & 7) - 1;
						if ((bytes[by3] >> p3) & 1) {
							used = 1;
							break;
						}
					}
					int by3 = end >> 3, p3 = 8 - (end & 7) - 1;
					if ((bytes[by3] >> p3) & 1)
						used = 1;
				}
				if (used) {
					int k;
					for (k = end; k >= i - 1; k--) {
						int by3 = k >> 3, p3 = 8 - (k & 7) - 1;
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
					if (m3 + i < 12) {
						c->bytes[1] |= 16;
						return *c;
					}
				}
				for (int k = i, l = 12 - m3; l < 64; k++, l++) {
					int by2 = k >> 3, p2 = 8 - (k & 7) - 1;
					int by3 = l >> 3, p3 = 8 - (l & 7) - 1;
					set (c, by3, p3, (bytes[by2] >> p2) & 1);
				}
			}
			return *c;
		}
		else {
			uc bytes[NUM_OF_BYTES << 1];
			for (int i = 0; i < 15; i++)
				bytes[i] = 0;
			int m1 = calexp (a), m2 = calexp (b);
			//printf ("%d %d\n",m1,m2);
			bytes[1] |= 16;
			for (int i = 12; i < 64; i++)
				for (int j = 12; j < 64; j++) {
					int by = i >> 3, p = 8 - (i & 7) - 1;
					int by2 = j >> 3, p2 = 8 - (j & 7) - 1;
					if ((a->bytes[by] >> p) & 1 && (b->bytes[by2] >> p2) & 1) {
						for (int pos = i + j - 11; pos > 0; pos--) {
							int by3 = pos >> 3, p3 = 8 - (pos & 7) - 1;
							if ((bytes[by3] >> p3) & 1)
								bytes[by3] -= 1 << p3;
							else {
								bytes[by3] |= 1 << p3;
								break;
							}
						}
					}
				}
			for (int i = 12; i < 64; i++) {
				int by = i >> 3, p = 8 - (i & 7) - 1;
				if ((a->bytes[by] >> p) & 1) {
					for (int pos = i; pos > 0; pos--) {
						int by3 = pos >> 3, p3 = 8 - (pos & 7) - 1;
						if ((bytes[by3] >> p3) & 1)
							bytes[by3] -= 1 << p3;
						else {
							bytes[by3] |= 1 << p3;
							break;
						}
					}
				}
			}
			for (int i = 12; i < 64; i++) {
				int by = i >> 3, p = 8 - (i & 7) - 1;
				if ((b->bytes[by] >> p) & 1) {
					for (int pos = i; pos > 0; pos--) {
						int by3 = pos >> 3, p3 = 8 - (pos & 7) - 1;
						if ((bytes[by3] >> p3) & 1)
							bytes[by3] -= 1 << p3;
						else {
							bytes[by3] |= 1 << p3;
							break;
						}
					}
				}
			}
			/*for (int i = 0; i < 16; i++) {
				for (int j = 7; j >= 0; j--)
					printf ("%d", (bytes[i] >> j) & 1);
				printf (" ");
			}
			puts ("");*/
			int i;
			for (i = 1; i < 128; i++) {
				int by = i >> 3, p = 8 - (i & 7) - 1;
				if ((bytes[by] >> p) & 1)
					break;
			}
			int ed = i + 52;
			int used = 0;
			int by = (ed + 1) >> 3, p = 8 - ((ed + 1) & 7) - 1;
			if ((bytes[by] >> p) & 1) {
				int bj = 0;
				for (int j = ed + 2; j < 128; j++) {
					int by2 = j >> 3, p2 = 8 - (j & 7) - 1;
					if ((bytes[by2] >> p2) & 1) {
						bj = 1;
						break;
					}
				}
				if (bj)
					used = 1;
				else {
					int by2 = ed >> 3, p2 = 8 - (ed & 7) - 1;
					if ((bytes[by2] >> p2) & 1)
						used = 1;
				}
			}
			if (used) {
				int j;
				for (j = ed; j >= i - 1; j--) {
					int by2 = j >> 3, p2 = 8 - (j & 7) - 1;
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
			//printf ("%d\n", i);
			struct double_n *c = &dc;
			clear (c);
			c->bytes[0] = (a->bytes[0] & 128) ^ (b->bytes[0] & 128);
			int m3 = m1 + m2 - Bias + 11 - i;
			if (m3 >= 2047) {
				setinf (c);
				return *c;
			}
			//printf ("%d\n", m3);
			if (m3 > 0) {
				c->bytes[0] |= m3 >> 4;
				c->bytes[1] |= (m3 & 127) << 4;
				for (int k = i+1, l = 12; l < 64; k++, l++) {
					int by2 = k >> 3, p2 = 8 - (k & 7) - 1;
					int by3 = l >> 3, p3 = 8 - (l & 7) - 1;
					set (c, by3, p3, (bytes[by2] >> p2) & 1);
				}
			}
			else {
			//puts("aaa");
				if (i - m3 - 1 > 64)
					return *c;
				int end = m3 + i + 51;
				int by2 = (end + 1) >> 3, p2 = 8 - ((end + 1) & 7) - 1;
				int used = 0;
				if ((bytes[by2] >> p2) & 1) {
					for (int k = end + 2; k < 128; k++) {
						int by3 = k >> 3, p3 = 8 - (k & 7) - 1;
						if ((bytes[by3] >> p3) & 1) {
							used = 1;
							break;
						}
					}
					int by3 = end >> 3, p3 = 8 - (end & 7) - 1;
					if ((bytes[by3] >> p3) & 1)
						used = 1;
				}
				if (used) {
					int k;
					for (k = end; k >= i + m3 - 1; k--) {
						int by3 = k >> 3, p3 = 8 - (k & 7) - 1;
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
					if (i - m3 < 12) {
						c->bytes[1] |= 16;
						return *c;
					}
				}
				if (i - m3 >= 64) {
					//puts ("orzwwx");
					//printf("%p", c);
					return *c;
				}
				/*for (int i = 0; i < 16; i++) {
					for (int j = 7; j >= 0; j--)
						printf ("%d", (bytes[i] >> j) & 1);
					printf (" ");
				}
				puts ("");*/
				for (int k = i, l = 12 - m3; l < 64; k++, l++) {
					int by2 = k >> 3, p2 = 8 - (k & 7) - 1;
					int by3 = l >> 3, p3 = 8 - (l & 7) - 1;
					set (c, by3, p3, (bytes[by2] >> p2) & 1);
				}
				//debug (c);
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
		a->bytes[0] ^= b->bytes[0] & 128;
		return *a;
	}
	if (is_inf (b)) {
		a->bytes[0] ^= b->bytes[0] & 128;
		a->bytes[0] &= 128;
		for (int i = 1; i < 8; i++)
			a->bytes[i] = 0;
		return *a;
	}
	if (allzero (a, 1)) {
		a->bytes[0] ^= b->bytes[0] & 128;
		if (allzero (b, 1)) {
			a->bytes[0] |= 127;
			a->bytes[1] |= 255 - 15 + 1;
		}
		return *a;
	}
	if (allzero (b, 1)) {
		a->bytes[0] ^= b->bytes[0] & 128;
		a->bytes[0] |= 127;
		a->bytes[1] = 255 - 15;
		for (int i = 2; i < 8; i++)
			a->bytes[i] = 0;
		return *a;
	}
	uc abytes[NUM_OF_BYTES << 1];
	uc bbytes[NUM_OF_BYTES];
	uc cbytes[NUM_OF_BYTES << 1];
	for (int i = 0; i < 16; i++)
		abytes[i] = 0;
	for (int i = 0; i < 8; i++)
		bbytes[i] = 0;
	for (int i = 0; i < 16; i++)
		cbytes[i] = 0;
	int m1, m2;
	if (iszero (a))
		m1 = 1;
	else {
		m1 = ((a->bytes[0] & 127) << 4) + (a->bytes[1] >> 4);
		abytes[1] |= 16;
	}
	for (int i = 12; i < 64; i++) {
		int by = i >> 3, p = 8 - (i & 7) - 1;
		abytes[by] |= ((a->bytes[by] >> p) & 1) << p;
	}
	if (iszero (b))
		m2 = 1;
	else {
		m2 = ((b->bytes[0] & 127) << 4) + (b->bytes[1] >> 4);
		bbytes[1] |= 16;
	}
	for (int i = 12; i < 64; i++) {
		int by = i >> 3, p = 8 - (i & 7) - 1;
		bbytes[by] |= ((b->bytes[by] >> p) & 1) << p;
	}
	int pos1, pos2;
	for (pos1 = 11; pos1 < 64; pos1++) {
		int by = pos1 >> 3, p = 8 - (pos1 & 7) - 1;
		if ((abytes[by] >> p) & 1)
			break;
	}
	for (pos2 = 11; pos2 < 64; pos2++) {
		int by = pos2 >> 3, p = 8 - (pos2 & 7) - 1;
		if ((bbytes[by] >> p) & 1)
			break;
	}
	/*for (int i = 0; i < 16; i++) {
		for (int j = 7; j >= 0; j--)
			printf ("%d", (abytes[i] >> j) & 1);
		printf (" ");
	}
	puts ("");
	for (int i = 0; i < 8; i++) {
		for (int j = 7; j >= 0; j--)
			printf ("%d", (bbytes[i] >> j) & 1);
		printf (" ");
	}
	puts ("");
	printf ("%d %d\n", pos1, pos2);*/
	for (int i = pos1; i < 128; i++) {
		int used = 1;
		int by = (i - 1) >> 3, p = 8 - ((i - 1) & 7) - 1;
		if (!((abytes[by] >> p) & 1)) {
			int w1, w2;
			for (int j = i, k = pos2; j < 128 || k < 64; j++, k++) {
				int by = j >> 3, p = 8 - (j & 7) - 1;
				int by2 = k >> 3, p2 = 8 - (k & 7) - 1;
				if (j < 128) w1 = ((abytes[by] >> p) & 1);
				else w1 = 0;
				if (k < 64) w2 = ((bbytes[by2] >> p2) & 1);
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
			if (i + 64 - pos2 > 128) {
				int bj = 0;
				int ed = pos2 + 128 - i;
				int by = ed >> 3, p = 8 - (ed & 7) - 1;
				if ((bbytes[by] >> p) & 1) {
					int by2 = 127 >> 3, p2 = 8 - (127 & 7) - 1;
					int by3 = (ed - 1) >> 3, p3 = 8 - ((ed - 1) & 7) - 1;
					if (((abytes[by2] >> p2) & 1) ^ ((bbytes[by3] >> p3) & 1))
						bj = 1;
					else {
						for (int j = ed + 1; j < 64; j++) {
							int by4 = j >> 3, p4 = 8 - (j & 7) - 1;
							if ((bbytes[by4] >> p4) & 1) {
								bj = 1;
								break;
							}
						}
					}
				}
				for (int j = 127, k = ed - 1; j >= i - 1; j--, k--) {
					int by2 = j >> 3, p2 = 8 - (j & 7) - 1;
					int by3 = k >> 3, p3 = 8 - (k & 7) - 1;
					int w1 = (abytes[by2] >> p2) & 1;
					int w2 = (bbytes[by3] >> p3) & 1;
					int res = w1 - w2 - bj;
					if (res < 0) {
						res += 2; bj = 1;
					}
					else
						bj = 0;
					abytes[by2] &= 255 - (1 << p2);
					abytes[by2] |= res << p2;
				}
			}
			else {
				int bj = 0;
				for (int j = i + 63 - pos2, k = 63; j >= i - 1; j--, k--) {
					int by2 = j >> 3, p2 = 8 - (j & 7) - 1;
					int by3 = k >> 3, p3 = 8 - (k & 7) - 1;
					int w1 = (abytes[by2] >> p2) & 1;
					int w2 = (bbytes[by3] >> p3) & 1;
					int res = w1 - w2 - bj;
					if (res < 0) {
						res += 2; bj = 1;
					}
					else
						bj = 0;
					abytes[by2] &= 255 - (1 << p2);
					abytes[by2] |= res << p2;
				}
			}
			int tmp = i - pos1 + 11;
			int by = tmp >> 3, p = 8 - (tmp & 7) - 1;
			cbytes[by] |= 1 << p;
		}
	}
	/*for (int i = 0; i < 16; i++) {
		for (int j = 7; j >= 0; j--)
			printf ("%d", (cbytes[i] >> j) & 1);
		printf (" ");
	}

	puts ("");*/
	/*for (int i = 0; i < 16; i++) {
		for (int j = 7; j >= 0; j--)
			printf ("%d", (cbytes[i] >> j) & 1);
		printf (" ");
	}
	puts ("");*/
	//printf ("%d %d\n", m1, m2);
	if (!(cbytes[1] & 16)) {
		m1--;
		for (int i = 11, j = 12; j < 128; i++, j++) {
			int by = i >> 3, p = 8 - (i & 7) - 1;
			int by2 = j >> 3, p2 = 8 - (j & 7) - 1;
			cbytes[by] &= 255 - (1 << p);
			cbytes[by] |= ((cbytes[by2] >> p2) & 1) << p;
		}
		cbytes[15] &= 254;
	}
	if (cbytes[8] >> 7) {
		int used = 0;
		if (cbytes[7] & 1)
			used = 1;
		for (int i = 65; i < 128; i++) {
			int by = i >> 3, p = 8 - (i & 7) - 1;
			if ((cbytes[by] >> p) & 1) {
				used = 1;
				break;
			}
		}
		if (used) {
			for (int i = 63; i > 0; i--) {
				int by = i >> 3, p = 8 - (i & 7) - 1;
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
	c->bytes[0] = (a->bytes[0] & 128) ^ (b->bytes[0] & 128);
	int m3 = m1 - m2 + Bias - pos1 + pos2;
	//cout<<m1<<" "<<m2<<" "<<m3<<endl;
	if (cbytes[1] & 32)
		m3++;
	if (m3 >= 2047) {
		setinf (c);
		return *c;
	}
	if (m3 > 0) {
		c->bytes[0] |= m3 >> 4;
		c->bytes[1] |= (m3 & 127) << 4;
		for (int i = 12; i < 64; i++) {
			int by = i >> 3, p = 8 - (i & 7) - 1;
			set (c, by, p, (cbytes[by] >> p) & 1);
		}
		//c.debug ();
		return *c;
	}
	else {
		int mov = 1 - m3;
		if (mov > 52)
			return *c;
		int ed = 64 - mov;
		int by = ed >> 3, p = 8 - (ed & 7) - 1;
		if ((cbytes[by] >> p) & 1) {
			int used = 0;
			int by2 = (ed - 1) >> 3, p2 = 8 - ((ed - 1) & 7) - 1;
			if ((cbytes[by2] >> p2) & 1)
				used = 1;
			for (int i = ed + 1; i < 64; i++) {
				int by3 = i >> 3, p3 = 8 - (i & 7) - 1;
				if ((cbytes[by3] >> p3) & 1) {
					used = 1;
					break;
				}
			}
			if (used) {
				for (int i = 63; i > 0; i--) {
					int by3 = i >> 3, p3 = 8 - (i & 7) - 1;
					if ((cbytes[by3] >> p3) & 1)
						cbytes[by3] -= 1 << p3;
					else {
						cbytes[by3] |= 1 << p3;
						used = 0;
						break;
					}
				}
			}
			if (cbytes[1] & 32) {
				mov--;
				if (!mov) {
					c->bytes[1] |= 16;
					return *c;
				}
				else {
					int by = (11 + mov) >> 3, p = 8 - ((11 + mov) & 7) - 1;
					c->bytes[by] |= 1 << p;
					return *c;
				}
			}
		}
		c->bytes[1] |= 16;
		for (int i = 11, j = 11 + mov; j < 64; i++, j++) {
			int by2 = i >> 3, p2 = 8 - (i & 7) - 1;
			int by3 = j >> 3, p3 = 8 - (j & 7) - 1;
			set (c, by3, p3, (cbytes[by2] >> p2) & 1);
		}
		return *c;
	}
}
int main () {
	//freopen ("double.in", "r", stdin);
	//freopen ("double.out", "w", stdout);
	for (int _ = 1; _ <= 10000; _++) {
		//printf ("%d", _);
		double a, b;
		char c;
		/*char s;
		s = getchar ();
		while (s == ' ') s = getchar ();
		for (int i = 0; i < 64; i++) {
			int by2 = i >> 3, p2 = 8 - (i & 7) - 1;
			set (&da, by2, p2, s - 48);
			s = getchar ();
		}
		while (s == ' ') s = getchar ();
		c = s;//printf ("%c", c);
		s = getchar ();
		while (s == ' ') s = getchar ();
		for (int i = 0; i < 64; i++) {
			int by2 = i >> 3, p2 = 8 - (i & 7) - 1;
			set (&db, by2, p2, s - 48);
			s = getchar ();
		}
		clear (&dc);
		a = trans (&da);
		b = trans (&db);*///printf ("%c\n", c);
		scanf ("%lf %c %lf", &a, &c, &b);
		da = chu (a), db = chu (b);
		//debug(&da);debug(&db);
		if (c == '+'){
			struct double_n tmp = add (&da, &db);
			//printf ("%.5lf\n", a + b);
			printf ("%.5lf\n", trans (&tmp));
		}
		if (c == '-'){
			struct double_n tmp = sub (&da, &db);
			//printf ("%.5lf\n", a - b);
			printf ("%.5lf\n", trans (&tmp));
		}
		if (c == '*'){
			struct double_n tmp = mul (&da, &db);//puts ("...");
			//printf ("%.5lf\n", a * b);
			printf ("%.5lf\n", trans (&tmp));
		}
		if (c == '/'){
			struct double_n tmp = divi (&da, &db);
			printf ("%.5lf\n", a / b);
			//printf ("%.5lf\n", trans (&tmp));
		}
	}
	//da.debug (), db.debug ();
	//printf ("%.12lf\n", a / b);
	//struct struct double_n dd = chu (a / b);
	//dd.debug ();
	//(da * db).debug ();
	//printf ("%.12lf\n", trans (da / db));
	//printf ("%.12lf\n%.12lf\n", trans (da), trans (db));
}

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
