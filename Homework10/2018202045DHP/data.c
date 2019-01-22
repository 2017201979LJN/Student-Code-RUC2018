#include <stdio.h>
#include <stdlib.h>
#define N 300000
#define LIM 20000000
int def[LIM + 10];

int main(){
	freopen("data.in", "w", stdout);
	printf("%d\n", N);
	for(int i=1; i<=N; i++){
		int val = rand()%LIM;
		while(def[val]) {
			//fprintf(stderr, "%d\n crashed", val);
			//if(val == (-8319593) + LIM/2) fprintf(stderr, "err\n");
			val = rand()%LIM;
		}
		def[val] = 1;
		char ch = rand()%52;
		if(ch <= 25) ch += 'A';
		else ch = ch - 26 + 'a';
		printf("%.3lf %c\n",(val-LIM/2) / 1000.0, ch);
	}
	return 0;
}
