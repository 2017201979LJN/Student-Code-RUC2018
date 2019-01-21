#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "avl.h"

#define size_T 101000
struct Tree
{
	int size;
	struct Data data;
	int ls, rs;
}T[size_T];

void buildtree(int *num, FILE *op)
{
	int d;
	char a, b;
	static int cnt = 0;
	*num = cnt++;
	fread(&d, sizeof(int), 1, op);
	fread(&a, sizeof(char), 1, op);
	fread(&b, sizeof(char), 1, op);
	T[*num].data.value1 = (double)d/1000;
	T[*num].data.value2 = a;
	T[*num].ls = -1;
	T[*num].rs = -1;
	T[*num].size = 1;
	if (b & 2)
	{
		buildtree(&T[*num].ls, op);
		T[*num].size += T[ T[*num].ls ].size;
	}
	if (b & 1)
	{
		buildtree(&T[*num].rs, op);
		T[*num].size += T[ T[*num].rs ].size;
	}
	//if (*num<=10)printf("%.3lf %d %d %d %d %d\n", d, a, b, *num, T[*num].ls, T[*num].rs);
}
int check(struct node *tree, int num)
{
	//printf("%.3lf %d %d %.3lf %d %d \n", tree -> data -> value1, tree -> data -> value2, tree -> size, T[num].data.value1, T[num].data.value2, T[num].size);
	if (tree == NULL && num == -1) return 1;
	if (tree == NULL || num == -1) return 0;
	if (tree -> size != T[num].size) return 0;
	return check(tree -> left, T[num].ls) & check(tree -> right, T[num].rs);
}

int main()
{
	freopen("data.in", "r", stdin);
	int n;
	scanf("%d", &n);
	struct node *root = NULL;
	struct Data data;
	for(int i = 1; i <= n; i++)
	{
		scanf("%lf%hhd", &data.value1, &data.value2);
		root = insert(root, data);
	}
	FILE *op = fopen("result.txt", "r");
	int Root;
	buildtree(&Root, op);
	fclose(op);
	if( check(root, Root) == 1) printf("AC\n");
	else printf("WA\n");
	return 0;
}
