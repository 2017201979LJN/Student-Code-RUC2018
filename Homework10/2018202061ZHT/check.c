#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "AVL.h"
#define MAXN 100005
int n, sz;
struct tree {
	int size;
	struct data d;
	int lc, rc;
}t[MAXN];

void build(int *x, FILE *op)
{
	int v1;
	char v2;
	*x = ++sz;
	fread (&v1, sizeof(int), 1, op);
	fread (&v2, sizeof(char), 1, op);
	t[*x].d.v1 = (double)v1/1000.0;
	t[*x].d.v2 = v2 >> 2;
	t[*x].lc = -1;
	t[*x].rc = -1;
	t[*x].size = 1;
	if (v2 & 1)
	{
		build(&t[*x].lc, op);
		t[*x].size += t[ t[*x].lc ].size;
	}
	if (v2 & 2)
	{
		build(&t[*x].rc, op);
		t[*x].size += t[ t[*x].rc ].size;
	}
}
int check(struct node *x, int y)
{
	if (x == nil && y == -1) return 1;
	if (x == nil || y == -1) return 0;
	if (x -> size != t[y].size) return 0;
	return check(x->ch[0], t[y].lc) & check(x->ch[1], t[y].rc);
}
int main() {
	freopen ("tree.in", "r", stdin);
	scanf ("%d", &n);
	nil = newnode ();
	root = nil;
	for (int i = 1; i <= n; i++) {
		double v1;char v2;
		struct data d;
		scanf ("%lf%hhd", &v1, &v2);
		d.v1 = v1; d.v2=v2;
		root = insert (root, d);
	}
	fclose(stdin);
	FILE *op = fopen ("tree.out", "r");
	int rt;
	build(&rt, op);
	if(check(root, rt) == 1) printf("YES\n");
	else printf("NO\n");
	fclose (op);
	return 0;
}
