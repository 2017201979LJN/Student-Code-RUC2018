#include <stdio.h>
#include <string.h>
#include <math.h>

#define MAXN 10010
#define true 1
#define false 0
typedef unsigned char uc;

struct data
{
	int x, y, z;
};

void _swap(void *a, void *b, size_t width)
{
	uc *temp1 = (uc*) a;
	uc *temp2 = (uc*) b;
	uc temp;
	while (width--)
	{
		temp = *temp1;
		*temp1 = *temp2;
		*temp2 = temp;
		temp1 ++;
		temp2 ++;
	}
}
void out(void *a, int width)
{
//	printf("value at %p:", a);
	uc *temp = (uc*) a;
	while (width--)
	{
		printf("%d", *temp);
		temp ++;
	}
	printf(" ");
}
void _qsort(void *base, int num, int width, int (*fcmp) (const void *, const void *) )
{
	if (num == 1) return ;
	void *midpl = (uc*)(base + (num / 2) * width);
//	struct data mid = *midpl;
/*	printf("%d\n", mid);
	void *pl = (uc*)base;
	for (int i = 1; i <= num; i++)
	{
		out((int*)pl, width);
		pl += width;
	}
	printf("\n");*/
	void *i = (struct data*)base, *j = (struct data*)(base + (num - 1) * width);
	do
	{
		while ((*fcmp) ((uc*)i, (uc*)midpl) == true) i += width;
		while ((*fcmp) ((uc*)midpl, (uc*)j) == true) j -= width;
		if (i <= j)
		{
			_swap(i, j, width);
			i += width;
			j -= width;
		}
	}while (i <= j);
/*	void *pl1 = (uc*)base;
	for (int k = 1; k <= num; k++)
	{
		out((int*)pl1, width);
		pl1 += width;
	}
	printf("\n");*/
	void *begin1 = (uc*) i, *begin2 = (uc*) base;
	if (i - base < (num - 1) * width ) _qsort( begin1, num - (i - base) / width, width, *fcmp);
	if (base - j < 0) _qsort( begin2, (j - base) / width + 1, width, *fcmp);
}
int cmp(const void *a, const void *b)
{
	struct data *aa = (struct data*)a, *bb = (struct data*)b;
	if ((*aa).x != (*bb).x) return (*aa).x < (*bb).x;
	if ((*aa).y != (*bb).y) return (*aa).y < (*bb).y;
	return (*aa).z < (*bb).z;
}
void put_in(int n, struct data *f)
{
	for (int i = 1; i <= n; i++)
	{
		scanf("%d", &f[i].x);
	}
	for (int i = 1; i <= n; i++)
	{
		scanf("%d", &f[i].y);
	}
	for (int i = 1; i <= n; i++)
	{
		scanf("%d", &f[i].z);
	}
}
void put_out(int n, struct data *f)
{
	for (int i = 1; i <= n; i++)
	{
		printf("%d ", f[i].x);
	}
	printf("\n");
	for (int i = 1; i <= n; i++)
	{
		printf("%d ", f[i].y);
	}
	printf("\n");
	for (int i = 1; i <= n; i++)
	{
		printf("%d ", f[i].z);
	}
	printf("\n");
}

int main()
{
	freopen("data.txt", "r", stdin);
	struct data f[MAXN];
	int n;
	scanf("%d", &n);
	put_in(n, f);
	_qsort(f + 1, n, sizeof(f[0]), cmp);
	put_out(n, f);
}
