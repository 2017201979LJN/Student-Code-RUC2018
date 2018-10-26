#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <time.h>
#include "summon.h"

#define CASES 100
#define LENGTH 2333;

int main(){
	char s[2333];
	srand(time(NULL));
	freopen("Double_Calculator.in", "w", stdout);
	for(int i = 1; i <= CASES; i++){
		formular_printer();
	}
	fclose(stdout);
	freopen("Double_Calculator.in", "r", stdin);
	freopen("checker.c", "w", stdout);
	printf("\
	#include <stdio.h>\n\
	#include <stdlib.h>\n\
	#include <string.h>\n\
	#include <memory.h>\n\
	#include <time.h>\n\
	int main() {\n\
	double a[110];\n");
	printf("freopen(\"Double_Calculator_c.out\", \"w\", stdout);\n");
	for(int i = 1; i <= CASES; i++){
		printf("a[%d] = ", i);
		scanf("%s", s+1);
		printf("%s;\n", s+1);
	}
	printf("for(int i = 1; i <= %d; i++) \n", CASES);
	printf("printf(\"%%.30lf\\n\", a[i]);\n");
	printf("return 0;\n");
	printf("}");
	return 0;
}
