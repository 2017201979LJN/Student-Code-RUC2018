#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct treap
{
	struct treap* c[2];
	int value, size, weight;
} treap;
typedef struct pair
{
	treap *l, *r;
} pair;
treap *root;

int getint()
{
	int x = 0;
	char ch = getchar();
	while (ch < '0' || ch > '9')
		ch = getchar();
	while (ch >= '0' && ch <= '9')
	{
		x = x * 10 + ch - '0';
		ch = getchar();
	}
	return x;
}

int size(treap *T)
{
	return T == NULL ? 0 : T->size;
}

void update(treap *T)
{
	T->size = 1 + size(T->c[0]) + size(T->c[1]);
	return;
}

treap* new(int x)
{
	treap *T = malloc(sizeof(treap));
	T->c[0] = T->c[1] = NULL;
	T->value = x;
	T->size = 1;
	T->weight = rand();
	return T;
}

treap* merge(treap *a, treap *b)
{
	if (a == NULL)
		return b;
	if (b == NULL)
		return a;
	if (a->weight < b->weight)
	{
		a->c[1] = merge(a->c[1], b);
		update(a);
		return a;
	}
	else
	{
		b->c[0] = merge(a, b->c[0]);
		update(b);
		return b;
	}
}

pair split(treap *T, int k)
{
	if (T == NULL)
		return (pair){NULL, NULL};
	pair res;
	treap *L = T->c[0], *R = T->c[1];
	if (k <= size(L))
	{
		res = split(L, k);
		T->c[0] = res.r;
		update(T);
		res.r = T;
	}
	else
	{
		res = split(R, k - size(L) - 1);
		T->c[1] = res.l;
		update(T);
		res.l = T;
	}
	return res;
}

pair split2(treap *T, int v)
{
	if (T == NULL)
		return (pair){NULL, NULL};
	treap *L = T->c[0], *R = T->c[1];
	pair res;
	if (v < T->value)
	{
		res = split2(L, v);
		T->c[0] = res.r;
		update(T);
		res.r = T;
	}
	else
	{
		res = split2(R, v);
		T->c[1] = res.l;
		update(T);
		res.l = T;
	}
	return res;
}

void insert(int v)
{
	pair a = split2(root, v);
	treap *T = new(v);
	root = merge(merge(a.l, T), a.r);
}

void delete(int v)
{
	pair a = split2(root, v);
	pair b = split2(a.l, v - 1);
	b.r = merge(b.r->c[0], b.r->c[1]);
	root = merge(merge(b.l, b.r), a.r);
}

int kth(int k)
{
	pair a = split(root, k);
	pair b = split(a.l, k - 1);
	int res = b.r->value;
	root = merge(merge(b.l, b.r), a.r);
	return res;
}

int rank(int v)
{
	pair a = split2(root, v - 1);
	int res = size(a.l);
	root = merge(a.l, a.r);
	return res;
}

int pre(int v)
{
	pair a = split2(root, v - 1);
	treap *T = a.l;
	if (T == NULL)
		return -1;
	while (T->c[1] != NULL)
		T = T->c[1];
	root = merge(a.l, a.r);
	return T->value;
}

int nxt(int v)
{
	pair a = split2(root, v);
	treap *T = a.r;
	if (T == NULL)
		return -1;
	while (T->c[0] != NULL)
		T = T->c[0];
	root = merge(a.l, a.r);
	return T->value;
}

int main()
{
	srand(time(0));
	int n = getint();
	for (int i = 1; i <= n; i++)
	{
		int type = getint(), x = getint();
		switch (type)
		{
			case 0:
			{
				insert(x);
				break;
			}
			case 1:
			{
				delete(x);
				break;
			}
			case 2:
			{
				printf("%d\n", kth(x));
				break;
			}
			case 3:
			{
				printf("%d\n", rank(x));
				break;
			}
			case 4:
			{
				printf("%d\n", pre(x));
				break;
			}
			case 5:
			{
				printf("%d\n", nxt(x));
				break;
			}
		}
	}
	return 0;
}