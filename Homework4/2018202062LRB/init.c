#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const char ope[2] = {'+', '*'};
int a[6];

int main()
{
	freopen("random.c", "w", stdout);
	srand(time(0));
	int n = rand() % 5 + 1;
	int m = rand() % (n * 4 + 1) + n;
	printf("#include <stdio.h>\n\nlong long wuyiyihanshu(");
	for (int i = 1; i <= n; i++)
	{
		a[i] = rand() % 101;
		printf("long long %c", 'a' + i - 1);
		if (i < n) printf(", ");
		else puts(")");
	}
	printf("{\n");
	while (m--)
	{
		int z = rand() % n + 1, x = rand() % (n + 3) + 1, y = rand() % (n + 3) + 1;
		char op = ope[rand() & 1];
		if (x > n && y > n)
		{
			int r = rand() & 1;
			if (r == 0) x = rand() % n + 1;
			else y = rand() % n + 1;
		}
		printf("\t%c = ", 'a' + z - 1);
		if (x > n) printf("%d", rand() % 101);
		else printf("%c", 'a' + x - 1);
		printf(" %c ", op);
		if (y > n) printf("%d", rand() % 101);
		else printf("%c", 'a' + y - 1);
		puts(";");
	}
	printf("\treturn %c;\n}", 'a' + rand() % n);
	printf("\n\nint main()\n{\n\tlong long ");
	for (int i = 1; i <= n; i++)
	{
		a[i] = rand() % 101;
		printf("%c = %d", 'a' + i - 1, a[i]);
		if (i < n) printf(", ");
		else puts(";");
	}
	printf("\twuyiyihanshu(");
	for (int i = 1; i < n; i++) printf("%c, ", 'a' + i - 1);
	printf("%c);\n\treturn 0;\n}", 'a' + n - 1);
	fclose(stdout);
	return 0;
}
