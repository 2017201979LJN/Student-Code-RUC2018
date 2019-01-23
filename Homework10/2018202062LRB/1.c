#include <stdio.h>

int main()
{
	FILE *fr = fopen("a.in", "r"), *fw = fopen("a.out", "w");
	if (fr == NULL)
	{
		puts("error");
		return 0;
	}
	int n;
	fscanf(fr, "%d\n", &n);
	double d;
	char c;
	while (n--)
	{
		fscanf(fr, "%lf %c", &d, &c);
		fwrite(&d, sizeof(double), 1, fw);
		fwrite(&c, sizeof(char), 1, fw);
	}
	return 0;
}
