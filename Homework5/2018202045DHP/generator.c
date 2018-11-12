#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define NUM 99983;

int main() {
	srand(time(NULL));
	int n = NUM;
	freopen("ssort.in", "w", stdout);
	printf("%d\n", n);
	for(int i = 1; i <= n; i++){
		printf("%d ", rand());
		if(i % 18 == 0) printf("\n");
	}

}
