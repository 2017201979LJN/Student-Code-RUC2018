#include <stdio.h>

void swap(void *x, void *y, size_t width) {
	while(width--){
		char temp = *(char *)x;
		*(char *)x = *(char *)y;
		*(char *)y = temp;
		x++; y++;
	}
}

void *query_mid(void *x, void *y, void *z, int (*fcmp) (const void *x,const void *y)) {
	int comp_1 = fcmp(x, y);
	int comp_2 = fcmp(y, z);
	int comp_3 = fcmp(x, z);
	if(comp_1 * comp_3 <= 0) return x;
	if(comp_2 * comp_3 <= 0) return z;
	return y;
}

void ssort (void *base, size_t nelem, size_t width, int (*fcmp) (const void *x, const void *y)) {
	if(nelem <= 1) return;
	void *end = base + (nelem - 1) * width;
	void *referv = query_mid(base, base + width * (nelem - 1), base + (nelem - 1) / 2 * width, fcmp); 
	swap(base, referv, width);
	void *left = base + width;
	void *right = base + (nelem - 1) * width;
	while(1) {
		while(left <= right && fcmp(left, base) <= 0) left += width;
		while(left <= right && fcmp(right, base) >= 0) right -= width;
		if(left > right) break;
		swap(left, right, width);
	} left -= width; right += width;
	if(left == base) {
		while(fcmp(base, left) >= 0 && nelem >= 1) {
			left += width;
			nelem --;
		}
		ssort(left, nelem, width, fcmp);
		return;
	}
	swap(base, left, width);
	left -= width;
	if(right > end) {
		right = end;
		while(fcmp(end, right) <= 0 && nelem >= 1) {
			right -= width;
			nelem --;
		}
		ssort(base, nelem, width, fcmp);
		return;
	}
	
	referv = left + width;
	while(left >= base && fcmp(referv, left) <= 0) left -= width;
	while(right <= end && fcmp(referv, right) >= 0) right += width;
	ssort(base, (left - base) / width + 1, width, fcmp);
	ssort(right, (end - right) / width + 1, width, fcmp);
}
