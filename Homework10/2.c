#include <stdio.h>

int main()
{
	FILE *fr = fopen("a.out", "r"), *fw = fopen("a.in", "w");
	if (fr == NULL)
	{
		puts("error");
		return 0;
	}
	fseek(fr, 0L, SEEK_END);
	int n = ftell(fr) / 9;
	fseek(fr, 0L, SEEK_SET);
	fprintf(fw, "%d\n", n);
	while (n--)
	{
		double f;
		char c;
		fread(&f, sizeof(double), 1, fr);
		fread(&c, sizeof(char), 1, fr);
		fprintf(fw, "%5.3lf %c\n", f, c);
	}
	return 0;
}
