#include <ctype.h>
#include <stdio.h>
#include <string.h>

char s[100010];
char used[5][5] = {{'%', 'r', 'd', 'i', '\0'}, {'%', 'r', 's', 'i', '\0'}, {'%', 'r', 
'd', 'x', '\0'}, {'%', 'r', 'c', 'x' , '\0'}, {'%', 'r', '8', '\0', '0'}};

int get1(int x)
{
	int ans = 0;
	while (x > 0)
	{
		x -= x & (-x);
		ans++;
	}
	return ans;
}

int main()
{
	freopen("random.c", "r", stdin);
	gets(s);
	gets(s);
	gets(s);
	printf("wuyiyihanshu:\n\tpushq   %rbp\n\tmovq    %rsp, %rbp\n");
	int len = strlen(s);
	int n = (len - 24) / 12;
	for (int i = 1; i <= n; i++) printf("\tmovq    %s, %d(%rbp)\n", used[i - 1], -i << 3);
	gets(s);
	while (1)
	{
		gets(s);
		if (s[1] == 'r')
		{
			printf("\tmovq    %d(%rbp), %rax\n\tpopq    %rbp\n\tret", -(s[8] - 'a' + 1) << 3);
			break;
		}
		char z = s[1], x, y;
		int x1, y1, xtype = 0, ytype = 0, begin = 5;
		if (isdigit(s[begin]))
		{
			xtype = 1;
			x1 = 0;
			while (isdigit(s[begin]))
			{
				x1 = x1 * 10 + s[begin] - '0';
				begin++;
			}
		}
		else x = s[begin], begin++;
		begin++;
		char op = s[begin];
		begin += 2;
		if (isdigit(s[begin]))
		{
			ytype = 1;
			y1 = 0;
			while (isdigit(s[begin]))
			{
				y1 = y1 * 10 + s[begin] - '0';
				begin++;
			}
		}
		else y = s[begin];
		if (xtype == 1 || ytype == 1)
		{
			if (ytype == 1)
			{
				char t = x;
				x = y;
				y = t;
				char t1 = x1;
				x1 = y1;
				y1 = t1;
			}
			if (op == '+')
			{
				if (x1 == 0) continue;
				if (z == y)
				{
					printf("\taddq    $%d, %d(%rbp)\n", x1, -(y - 'a' + 1) << 3);
				}
				else
				{
					printf("\tmovq    %d(%rbp), %rax\n", -(y - 'a' + 1) << 3);
					printf("\taddq    $%d, %rax\n\tmovq    %rax, %d(%rbp)\n", x1, -(z - 'a' + 1) << 3);
				}
			}
			else
			{
				if (x1 == 0) printf("\t movq    $0, %d(%rbp)\n", -(z - 'a' + 1) << 3);
				else if (x1 == 1) continue;
				int num1 = get1(x1);
				if (num1 == 1)
				{
					int exp = 0, v = 1;
					while (v < x1) exp++, v <<= 1;
					if (z == y)
					{
						if (exp == 1) printf("\tsalq    %d(%rbp)\n", -(y - 'a' + 1) << 3);
						else printf("\tsalq    $%d, %d(%rbp)\n", exp, -(y - 'a' + 1) << 3);
					}
					else
					{
						printf("\tmovq    %d(%rbp), %rax\n", -(y - 'a' + 1) << 3);
						if (exp == 1) printf("\tsalq    %rax\n");
						else printf("\tsalq    $%d, %rax\n", exp);
						printf("\tmovq    %rax, %d(%rbp)\n", -(z - 'a' + 1) << 3);
					}
				}
				else
				{
					printf("\tmovq    %d(%rbp), %rdx\n", -(y - 'a' + 1) << 3);
					printf("\tmovq    %rdx, %rax\n");
					while (num1 > 1)
					{
						int fir = 0, sec = 0;
						while (1 << fir + 1 <= x1) fir++;
						x1 -= 1 << fir;
						while (1 << sec + 1 <= x1) sec++;
						printf("\tsalq    $%d, %rax\n", fir - sec);
						printf("\taddq    %rdx, %rax\n");
						num1--;
					}
					int fir = 0;
					while (1 << fir + 1 <= x1) fir++;
					if (fir != 0) printf("\tsalq    $%d, %rax\n", fir);
					printf("\tmovq    %rax, %d(%rbp)\n", -(z - 'a' + 1) << 3);
				}
			}
		}
		else
		{
			if (x == y)
			{
				if (x == z && op == '+')
					printf("\tsalq    %d(%rbp)\n", -(x - 'a' + 1) << 3);
				else
				{ 
					printf("\tmovq    %d(%rbp), %rax\n", -(x - 'a' + 1) << 3);
					if (op == '+') printf("\taddq    ");
					else printf("\timulq   ");
					printf("%d(%rbp), %rax\n\tmovq    %rax, %d(%rbp)\n", -(x - 'a' + 1) << 3, -(z - 'a' + 1) << 3);
				}
			}
			else
			{
				printf("\tmovq    %d(%rbp), %rax\n", -(x - 'a' + 1) << 3);
				if (op == '+') printf("\taddq    ");
				else printf("\timulq   ");
				printf("%d(%rbp), %rax\n\tmovq    %rax, %d(%rbp)\n", -(y - 'a' + 1) << 3, -(z - 'a' + 1) << 3);
			}
		}
	}
	return 0;
}
