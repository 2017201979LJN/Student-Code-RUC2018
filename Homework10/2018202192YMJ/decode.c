#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

#include "treap.h"

#define NUM_OF_BITS 8

#define DEPTH 100

extern struct Treap_Node* Treap_Root;

static FILE* input;
static FILE* output;

static int *arr;

struct my_struct
{
	double d;
	char c;
};

int gen_int(int L, int R)
{
	return L + rand() % (R - L + 1);
}

int ceiling(int a, int b)
{
	return (a + b - 1) / b;
}

void decode(struct Treap_Node** u, int depth)
{
	static int cnt = 0;
	*u = malloc(sizeof(struct Treap_Node));
	struct my_struct* valuep;
	valuep = malloc(sizeof(struct my_struct));
	int x;
	char c;
	fread(&x, sizeof(int), 1, input);
	fread(&c, sizeof(char), 1, input);
	valuep->d = (double)x / 1000.0;
	valuep->c = c;
	(*u)->valuep = valuep;
	int lch_bit = *arr & (1 << cnt);
	int rch_bit = *arr & (1 << (cnt + 1));
	cnt += 2;
	if (cnt == sizeof(int) * NUM_OF_BITS)
	{
		cnt = 0;
		++arr;
	}
	if (lch_bit != 0)
	{
		decode(&((*u)->ch[TREAP_LCH]), depth + 1);
	}
	else
	{
		(*u)->ch[TREAP_LCH] = NULL;
	}
	if (rch_bit != 0)
	{
		decode(&((*u)->ch[TREAP_RCH]), depth + 1);
	}
	else
	{
		(*u)->ch[TREAP_RCH] = NULL;
	}
	int size = 1;
	if ((*u)->ch[TREAP_LCH] != NULL)
	{
		size += (*u)->ch[TREAP_LCH]->size;
	}
	if ((*u) -> ch[TREAP_RCH] != NULL)
	{
		size += (*u)->ch[TREAP_RCH]->size;
	}
	(*u)->size = size;
	(*u)->key = gen_int(UINT_MAX / DEPTH * depth,
				  		UINT_MAX / DEPTH * (depth + 1) - 1);
	(*u)->key = UINT_MAX - (*u)->key;
}

void rec(struct Treap_Node* u)
{
	if (u == NULL) return;
	fprintf(output, "%d\n", u -> size);
	rec(u -> ch[TREAP_LCH]);
	rec(u -> ch[TREAP_RCH]);
}

int main()
{
	input = fopen("compression.txt", "r");
	output = fopen("data.out", "w");
	int n;
	fread(&n, sizeof(int), 1, input);
	int cnt = ceiling(n * TREAP_NUM_OF_CHILD, NUM_OF_BITS * sizeof(int));
	arr = malloc(cnt * sizeof(int));
	int *memo = arr;
	fread(arr, sizeof(int), cnt, input);
	decode(&Treap_Root, 0);
	rec(Treap_Root);
	Treap_Destruct();
	free(memo);
	fclose(input);
	fclose(output);
	return 0;
}
