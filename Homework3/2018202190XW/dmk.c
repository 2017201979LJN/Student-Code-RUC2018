#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#define CASES 100
#define NUM_IN_CASE 20
double a[NUM_IN_CASE + 100];
char operator[NUM_IN_CASE + 100];
int hav_left[NUM_IN_CASE + 100];
int hav_right[NUM_IN_CASE + 100];
int main() {
	srand(time(0));
	for (int i = 1; i <= CASES; i++) {
		for (int j = 1; j <= NUM_IN_CASE; j++) {
			hav_left[j] = hav_right[j] = 0;
			unsigned char bytes[8];
			for (int k = 0; k < 8; k++) bytes[k] = rand() % 256;
			a[j] = *(double*) bytes;
		}
		for (int j = 1; j < NUM_IN_CASE; j++) {
			int rad = rand() % 4;
			switch (rad) {
				case 0:
					operator[j] = '+';
					break;
				case 1:
					operator[j] = '-';
					break;
				case 2:
					operator[j] = '*';
					break;
				case 3:
					operator[j] = '/';
					break;
				default:
					break;
			}
		}
		int num_of_bracket = rand() % 3;
		for (int j = 1; j <= num_of_bracket; j++) {
			int a = rand() % NUM_IN_CASE + 1;
			int b = rand() % NUM_IN_CASE + 1;
			if (a > b) {
				int t = a;
				a = b;
				b = t;
			}
			hav_left[a]++;
			hav_right[b]++;
		}
		for (int j = 1; j <= NUM_IN_CASE; j++) {
			for (int k = 1; k <= hav_left[j]; k++) printf("(");
			if (hav_right[j]) printf("%.5le", a[j]);
			else printf("%.5le ", a[j]);
			for (int k = 1; k <= hav_right[j]; k++) printf(") ");
			if (j != NUM_IN_CASE) printf("%c ", operator[j]);
		}
		printf("\n");
	} 
	return 0;
}
