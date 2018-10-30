#include <stdio.h>
int getline_(FILE* input, char s[],int lim){
	char c;
	int len = 0;
	while (fscanf(input, "%c", &c) != EOF && c != '\n' && len < lim - 1) {
        s[len++] = c;
    }
	s[len] = '\0';
	return len;
}
int main() {
	int rounding;
	//scanf("%d", &rounding);
	rounding = 50;
	printf("#include <stdio.h>\n");
	printf("double a;\n");
	printf("int main() {\n");
	FILE* input = fopen("data.in", "r");
	char c[1001];
	int len = 0;
	while (len = getline_(input, c, 1000)) {
		printf("\ta = ");
		for (int i = 0; i < len; i++) printf("%c", c[i]);
		printf(";\n");
		printf("\tprintf(\"%%.%dlE\\n\", a);\n\n", rounding);
	}
	printf("\treturn 0;\n");
	printf("}\n");
	fclose(input);
	return 0;
}

